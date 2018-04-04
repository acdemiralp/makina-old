#include <makina/renderer/backend/vulkan/render_tasks.hpp>

#include <makina/renderer/backend/glsl/shader_sources.hpp>
#include <makina/renderer/backend/vulkan/context.hpp>

namespace mak
{
namespace vulkan
{
fg::render_task<test_task_data>*                     add_test_render_task                    (renderer* framegraph)
{
  struct vertex_t
  {
    glm::vec3 vertex;
  };
  std::vector<vertex_t> vertices {{{-1.0f, 1.0f, 0.0f}},{{1.0f, 1.0f, 0.0f}},{{1.0f, -1.0f, 0.0f}},{{-1.0f, 1.0f, 0.0f}},{{-1.0f, -1.0f, 0.0f}}};
  const std::uint32_t   draw_count = 5;
 
  return framegraph->add_render_task<test_task_data>(
    "Test Pass",
    [&] (      test_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices = builder.create<buffer_resource>("Test Vertices", buffer_description{
        vertices.size() * sizeof(vertex_t), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer});
 
      data.pipeline = builder.create<pipeline_resource>("Test Pipeline", pipeline_description{
        glsl::test_vertex_shader  , 
        glsl::test_fragment_shader,
        vk::PrimitiveTopology::eTriangleStrip,
        vulkan::context().window_swapchains[0].render_pass, // Test task applies to the first window.
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(vertex_t, vertex))
        },
        sizeof vertex_t});
    },
    [=] (const test_task_data& data)
    {
      auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
      for(auto& window_swapchain : vulkan::context().window_swapchains)
      {
        command_buffer->begin           ();
        (*data.vertices->actual         ())->update<vertex_t>(0, {std::uint32_t(vertices.size()), vertices.data()}, command_buffer);
        command_buffer->beginRenderPass (
          window_swapchain.render_pass, 
          window_swapchain.swapchain->getFramebuffer(), 
          vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()), 
          {
            vk::ClearValue(std::array<float, 4>{0.1f, 0.1f, 0.1f}), 
            vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))
          }, 
          vk::SubpassContents::eInline);
        command_buffer->bindPipeline    (vk::PipelineBindPoint::eGraphics, *data.pipeline->actual());
        command_buffer->bindVertexBuffer(0, *data.vertices->actual(), 0);
        command_buffer->setViewport     (0, vk::Viewport(
          0.0f, 
          0.0f, 
          static_cast<float>(window_swapchain.swapchain->getExtent().width ), 
          static_cast<float>(window_swapchain.swapchain->getExtent().height), 
          0.0f, 
          1.0f));
        command_buffer->setScissor      (0, vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()));
        command_buffer->draw            (draw_count, 1, 0, 0);
        command_buffer->endRenderPass   ();
        command_buffer->end             ();
        vkhlf::submitAndWait(vulkan::context().graphics_queue, command_buffer);
      }
    });
}
fg::render_task<upload_scene_task_data>*             add_upload_scene_render_task            (renderer* framegraph)
{
  // Shader types with std430 alignment.
  struct _transform
  {
    glm::mat4    model         ;
  };
  struct _phong_material
  {
    glm::uvec4   use_texture   ; // ambient - diffuse - specular - unused
    glm::vec4    ambient       ; // w is unused.
    glm::vec4    diffuse       ; // w is unused.
    glm::vec4    specular      ; // w is shininess.
    glm::u64vec4 textures      ; // ambient - diffuse - specular - unused
  };
  struct _physically_based_material
  {
    glm::uvec4   use_texture   ; // albedo - metallicity - roughness - normal
    glm::uvec4   use_texture_2 ; // ambient occlusion - unused - unused - unused
    glm::vec4    albedo        ; // w is unused.
    glm::vec4    properties    ; // metallicity - roughness - unused - unused
    glm::u64vec4 textures      ; // albedo - metallicity - roughness - normal
    glm::u64vec4 textures_2    ; // ambient occlusion - unused - unused - unused
  };
  struct _camera
  {
    glm::mat4    view          ;
    glm::mat4    projection    ;
  };             
  struct _light  
  {              
    glm::uvec4   type          ; // y, z, w are unused.
    glm::vec4    color         ; // w is unused.
    glm::vec4    properties    ; // intensity - range - inner spot - outer spot
    glm::vec4    position      ; // w is unused.
    glm::vec4    direction     ; // w is unused.
  };

  const auto retained_vertices      = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Vertices"     , buffer_description{128e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer  });
  const auto retained_indices       = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Indices"      , buffer_description{ 64e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer   });
  const auto retained_transforms    = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Transforms"   , buffer_description{ 16e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer });
  const auto retained_materials     = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Materials"    , buffer_description{ 16e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer });
  const auto retained_cameras       = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Cameras"      , buffer_description{ 16e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer });
  const auto retained_lights        = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Lights"       , buffer_description{ 16e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer });
  const auto retained_draw_calls    = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Draw Calls"   , buffer_description{ 16e+6, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndirectBuffer});
  const auto retained_parameter_map = framegraph->add_retained_resource<parameter_map::description, parameter_map>         ("Scene Parameter Map", parameter_map::description());
  // Totals to 128 * 1 + 64 * 1 + 16 * 5 = 272 MB of GPU memory for buffers.

  std::vector<image_resource*> retained_images(64);
  for (auto i = 0; i < retained_images.size(); ++i)
    retained_images[i] = framegraph->add_retained_resource<image_description, std::shared_ptr<vkhlf::Image>>("Scene Texture " + boost::lexical_cast<std::string>(i), image_description{}); // 2048x2048
  // Totals to 64 * 16.77 = 1073 MB of GPU memory for textures.

  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [&] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      
    },
    [=] (const upload_scene_task_data& data)
    {
      
    });
}
fg::render_task<phong_task_data>*                    add_phong_render_task                   (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  return nullptr;
}
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  return nullptr;
}
fg::render_task<ui_task_data>*                       add_ui_render_task                      (renderer* framegraph, framebuffer_resource* target)
{
  return nullptr;
}
fg::render_task<present_task_data>*                  add_present_render_task                 (renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<present_task_data>(
    "Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const present_task_data& data)
    {
      vulkan::context().present_window_swapchains();
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}
