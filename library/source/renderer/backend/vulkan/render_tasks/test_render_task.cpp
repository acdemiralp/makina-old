#include <makina/renderer/backend/vulkan/render_tasks/test_render_task.hpp>

#include <makina/renderer/backend/glsl/test_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/test_vertex_shader.hpp>
#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace vulkan
{
fg::render_task<test_task_data>* add_test_render_task(renderer* framegraph)
{
  struct vertex_t
  {
    glm::vec3 vertex;
  };
  std::vector<vertex_t> vertices {{{-1.0f, 1.0f, 0.0f}},{{1.0f, 1.0f, 0.0f}},{{1.0f, -1.0f, 0.0f}},{{-1.0f, 1.0f, 0.0f}},{{-1.0f, -1.0f, 0.0f}}};
  const std::uint32_t   draw_count = 5;
 
  auto render_task = framegraph->add_render_task<test_task_data>(
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
  render_task->set_cull_immune(true);
  return render_task;
}
}
}
