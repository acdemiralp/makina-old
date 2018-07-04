#include <makina/renderer/backend/vulkan/render_tasks/ui_render_task.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <ImGui.h>

#include <makina/renderer/backend/glsl/ui_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/ui_vertex_shader.hpp>
#include <makina/resources/image.hpp>

namespace mak
{
namespace vulkan
{
void load_image(std::uint8_t* pixels, std::array<std::uint32_t, 2> dimensions, std::shared_ptr<vkhlf::Buffer> intermediate, const std::shared_ptr<vkhlf::Image> target, const std::size_t offset)
{
  auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
  command_buffer->begin               ();
  intermediate  ->update<std::uint8_t>(0, {dimensions[0] * dimensions[1] * 4u, pixels}, command_buffer);
  command_buffer->copyBufferToImage   (intermediate, target, vk::ImageLayout::eTransferDstOptimal, vk::BufferImageCopy(0, 0, 0, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 0), vk::Offset3D(0, 0, offset), vk::Extent3D(dimensions[0], dimensions[1], 1)));
  command_buffer->end                 ();
  vkhlf::submitAndWait                (vulkan::context().graphics_queue, command_buffer);
}

fg::render_task<ui_task_data>* add_ui_render_task(renderer* framegraph, framebuffer_resource* target)
{
  return framegraph->add_render_task<ui_task_data>(
    "UI Pass",
    [&] (      ui_task_data& data, fg::render_task_builder& builder)
    {
      std::uint8_t* pixels;
      std::int32_t  width, height;
      ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
  
      auto descriptor_set_layout = context().logical_device->createDescriptorSetLayout(std::vector<vkhlf::DescriptorSetLayoutBinding> 
      {
        vkhlf::DescriptorSetLayoutBinding {0, vk::DescriptorType::eStorageBufferDynamic, vk::ShaderStageFlagBits::eVertex  , nullptr},
        vkhlf::DescriptorSetLayoutBinding {1, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment, nullptr}
      });

      data.intermediates = builder.create<buffer_resource>  ("UI Intermediates", buffer_description {vk::DeviceSize(32e+6)           , vk::BufferUsageFlagBits::eTransferSrc , vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent});
      data.vertices      = builder.create<buffer_resource>  ("UI Vertices"     , buffer_description {vk::DeviceSize(4e+6)            , vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer});
      data.indices       = builder.create<buffer_resource>  ("UI Indices"      , buffer_description {vk::DeviceSize(4e+6)            , vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer });
      data.projection    = builder.create<buffer_resource>  ("UI Projection"   , buffer_description {vk::DeviceSize(sizeof glm::mat4), vk::BufferUsageFlagBits::eTransferDst                                         });
      data.image         = builder.create<image_resource>   ("UI Image"        , image_description  {vk::ImageType::e2D , vk::Extent3D(width, height, 1), vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled});
      data.sampler       = builder.create<sampler_resource> ("UI Sampler"      , sampler_description{vk::Filter::eLinear, vk::Filter::eLinear});
      data.pipeline      = builder.create<pipeline_resource>("UI Pipeline"     , pipeline_description{
        glsl::ui_vertex_shader,
        glsl::ui_fragment_shader,
        vk::PrimitiveTopology::eTriangleList,
        context().window_swapchains[0].render_pass, // Applies to the first window swapchain.
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat , offsetof(ImDrawVert, pos)),
          vk::VertexInputAttributeDescription(0, 1, vk::Format::eR32G32Sfloat , offsetof(ImDrawVert, uv )),
          vk::VertexInputAttributeDescription(0, 2, vk::Format::eR8G8B8A8Unorm, offsetof(ImDrawVert, col))
        },
        sizeof ImDrawVert,
        { 
          context().logical_device->allocateDescriptorSet(context().descriptor_pool, descriptor_set_layout) 
        },
        context().logical_device->createPipelineLayout(descriptor_set_layout, {})
      });
      data.target        = builder.write (target);
    },
    [=] (const ui_task_data& data)
    {
      auto& io = ImGui::GetIO();

      std::uint8_t* pixels;
      std::int32_t  width, height;
      io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

      context().logical_device->updateDescriptorSets(std::vector<vkhlf::WriteDescriptorSet>
      {
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 0u, 0u, 1u, vk::DescriptorType::eStorageBufferDynamic, nullptr, 
          vkhlf::DescriptorBufferInfo{*data.projection->actual(), 0, (*data.projection->actual())->getSize()}),
        vkhlf::WriteDescriptorSet(data.pipeline->description().descriptor_sets[0], 1u, 0u, 1u, vk::DescriptorType::eCombinedImageSampler,
          vkhlf::DescriptorImageInfo(*data.sampler->actual(), (*data.image->actual())->createImageView(vk::ImageViewType::e2DArray, vk::Format::eR8G8B8A8Unorm), vk::ImageLayout::eShaderReadOnlyOptimal), nullptr)
      }, {});
      
      load_image(pixels, {std::uint32_t(width), std::uint32_t(height)}, *data.intermediates->actual(), *data.image->actual(), 0);
      io.Fonts->TexID = reinterpret_cast<void*>(std::intptr_t((*data.image->actual())->operator vk::Image().operator VkImage()));

      const auto projection = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1.0f);
      
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
      command_buffer->bindIndexBuffer   (   *data.indices ->actual(), 0, vk::IndexType::eUint16);
      command_buffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, data.pipeline->description().pipeline_layout, 0u, data.pipeline->description().descriptor_sets, {0, 0});
      command_buffer->setViewport       (0, vk::Viewport(
        0.0f, 
        0.0f, 
        static_cast<float>(window_swapchain.swapchain->getExtent().width ), 
        static_cast<float>(window_swapchain.swapchain->getExtent().height), 
        0.0f, 
        1.0f));
      
      (*data.projection->actual())->update<glm::mat4>(0, {1u, &projection}, command_buffer);

      ImGui::Render();
      auto draw_data = ImGui::GetDrawData();
      for (auto i = 0; i < draw_data->CmdListsCount; ++i)
      {
        const ImDrawList* command_list = draw_data->CmdLists[i];
        std::uint32_t     index_offset = 0;
        
        (*data.vertices->actual())->update<ImDrawVert>(0, {static_cast<std::uint32_t>(command_list->VtxBuffer.Size * sizeof(ImDrawVert)), command_list->VtxBuffer.Data}, command_buffer);
        (*data.indices ->actual())->update<ImDrawIdx> (0, {static_cast<std::uint32_t>(command_list->IdxBuffer.Size * sizeof(ImDrawIdx )), command_list->IdxBuffer.Data}, command_buffer);

        for (auto j = 0; j < command_list->CmdBuffer.Size; ++j)
        {
          auto& command = command_list->CmdBuffer[j];
          command_buffer->setScissor(0u, vk::Rect2D(
            {std::int32_t (command.ClipRect.x )                    , std::int32_t (command.ClipRect.y)}, 
            {std::uint32_t(command.ClipRect.z - command.ClipRect.x), std::uint32_t(command.ClipRect.w - command.ClipRect.y)}));
          command_buffer->drawIndexed(command.ElemCount, 1, index_offset, 0, 0);
          index_offset += command.ElemCount;
        }
      }
      ImGui::NewFrame();
      command_buffer->endRenderPass();
      command_buffer->end          ();
      vkhlf::submitAndWait(context().graphics_queue, command_buffer);
    });
}
}
}
