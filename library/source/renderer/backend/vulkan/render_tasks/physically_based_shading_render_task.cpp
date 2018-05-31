#include <makina/renderer/backend/vulkan/render_tasks/physically_based_shading_render_task.hpp>

#include <makina/renderer/backend/glsl/triangle_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/triangle_physically_based_fragment_shader.hpp>

#include <vkhlf/DescriptorSet.h>

namespace mak
{
namespace vulkan
{
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  // Shader vertex type.
  struct _vertex
  {
    glm::vec3  position           ;
    glm::vec3  normal             ;
    glm::vec3  texture_coordinates;
    glm::uvec2 instance_attributes;
  };

  return framegraph->add_render_task<physically_based_shading_task_data>(
    "Physically Based Shading Pass",
    [&] (      physically_based_shading_task_data& data, fg::render_task_builder& builder)
    {
      auto descriptor_set_layout = context().logical_device->createDescriptorSetLayout(std::vector<vkhlf::DescriptorSetLayoutBinding> 
      {
        vkhlf::DescriptorSetLayoutBinding {0, vk::DescriptorType::eStorageBuffer       , vk::ShaderStageFlagBits::eVertex                                     , nullptr},
        vkhlf::DescriptorSetLayoutBinding {1, vk::DescriptorType::eStorageBuffer       ,                                    vk::ShaderStageFlagBits::eFragment, nullptr},
        vkhlf::DescriptorSetLayoutBinding {2, vk::DescriptorType::eStorageBuffer       , vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, nullptr},
        vkhlf::DescriptorSetLayoutBinding {3, vk::DescriptorType::eStorageBuffer       ,                                    vk::ShaderStageFlagBits::eFragment, nullptr},
        vkhlf::DescriptorSetLayoutBinding {4, vk::DescriptorType::eCombinedImageSampler,                                    vk::ShaderStageFlagBits::eFragment, nullptr}
      });

      data.vertices      = builder.read(scene_data.vertices     );
      data.indices       = builder.read(scene_data.indices      );
      data.transforms    = builder.read(scene_data.transforms   );
      data.materials     = builder.read(scene_data.materials    );
      data.cameras       = builder.read(scene_data.cameras      );
      data.lights        = builder.read(scene_data.lights       );
      data.draw_calls    = builder.read(scene_data.draw_calls   );
      data.images        = builder.read(scene_data.images       );
      data.sampler       = builder.read(scene_data.sampler      );
      data.parameter_map = builder.read(scene_data.parameter_map);
      data.pipeline      = builder.create<pipeline_resource>("Physically Based Shading Pipeline", pipeline_description{
        glsl::triangle_vertex_shader,
        glsl::triangle_physically_based_fragment_shader,
        vk::PrimitiveTopology::eTriangleList,
        context().window_swapchains[0].render_pass, // Applies to the first window swapchain.
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(_vertex, position           )),
          vk::VertexInputAttributeDescription(0, 1, vk::Format::eR32G32B32A32Sfloat, offsetof(_vertex, normal             )),
          vk::VertexInputAttributeDescription(0, 2, vk::Format::eR32G32B32Sfloat   , offsetof(_vertex, texture_coordinates)),
          vk::VertexInputAttributeDescription(0, 3, vk::Format::eR32G32Uint        , offsetof(_vertex, instance_attributes))
        },
        sizeof _vertex,
        { 
          context().logical_device->allocateDescriptorSet(context().descriptor_pool, descriptor_set_layout)
        },
        context().logical_device->createPipelineLayout(descriptor_set_layout, {})
      });
      data.target        = builder.write(target);
    },
    [=] (const physically_based_shading_task_data& data)
    {
      context().logical_device->updateDescriptorSets(std::vector<vkhlf::WriteDescriptorSet>
      {
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 0u, 0u, 1u, vk::DescriptorType::eStorageBuffer, nullptr, 
          vkhlf::DescriptorBufferInfo{*data.transforms->actual(), 0, (*data.transforms->actual())->getSize()}),
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 1u, 0u, 1u, vk::DescriptorType::eStorageBuffer, nullptr, 
          vkhlf::DescriptorBufferInfo{*data.materials ->actual(), 0, (*data.materials ->actual())->getSize()}),
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 2u, 0u, 1u, vk::DescriptorType::eStorageBuffer, nullptr, 
          vkhlf::DescriptorBufferInfo{*data.cameras   ->actual(), 0, (*data.cameras   ->actual())->getSize()}),
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 3u, 0u, 1u, vk::DescriptorType::eStorageBuffer, nullptr,
          vkhlf::DescriptorBufferInfo{*data.lights    ->actual(), 0, (*data.lights    ->actual())->getSize()}),
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 4u, 0u, 1u, vk::DescriptorType::eCombinedImageSampler,
          vkhlf::DescriptorImageInfo(*data.sampler->actual(), (*data.images ->actual())->createImageView(vk::ImageViewType::e2DArray, vk::Format::eR8G8B8A8Unorm), vk::ImageLayout::eShaderReadOnlyOptimal), nullptr)
      }, {});
      
      auto command_buffer   = context().command_pool->allocateCommandBuffer();
      auto window_swapchain = context().window_swapchains[0];  
      command_buffer->begin           ();
      command_buffer->beginRenderPass (
        window_swapchain.render_pass, 
        window_swapchain.swapchain->getFramebuffer(), 
        vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()), 
        {
          vk::ClearValue(std::array<float, 4>{0.1f, 0.1f, 0.1f, 0.0f}), 
          vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))
        }, 
        vk::SubpassContents::eInline);
      command_buffer->bindPipeline      (vk::PipelineBindPoint::eGraphics, *data.pipeline->actual());
      command_buffer->bindVertexBuffer  (0, *data.vertices->actual(), 0);
      command_buffer->bindIndexBuffer   (   *data.indices ->actual(), 0, vk::IndexType::eUint32);
      command_buffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, data.pipeline->description().pipeline_layout, 0u, data.pipeline->description().descriptor_sets, {});
      command_buffer->setViewport       (0, vk::Viewport(
        0.0f, 
        0.0f, 
        static_cast<float>(window_swapchain.swapchain->getExtent().width ), 
        static_cast<float>(window_swapchain.swapchain->getExtent().height), 
        0.0f, 
        1.0f));
      command_buffer->setScissor         (0, vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()));
      command_buffer->drawIndexedIndirect(*data.draw_calls->actual(), 0, data.parameter_map->actual()->get<std::uint32_t>("draw_count"), sizeof vk::DrawIndexedIndirectCommand);
      command_buffer->endRenderPass      ();
      command_buffer->end                ();
      vkhlf::submitAndWait(context().graphics_queue, command_buffer);
    });
}
}
}
