#include <makina/renderer/backend/vulkan/render_tasks.hpp>

#include <makina/renderer/backend/vulkan/context.hpp>

namespace mak
{
namespace vulkan
{
fg::render_task<test_task_data>*    add_test_render_task   (renderer* framegraph)
{
  return framegraph->add_render_task<test_task_data>(
    "Test Pass",
    [ ] (      test_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const test_task_data& data)
    {
 //     struct vertex_t
 //     {
 //       glm::vec3 vertex;
 //     };
 //     std::vector<vertex_t> vertices{ { { -1.0f, 1.0f, 0.0f } },{ { 1.0f, 1.0f, 0.0f } },{ { 1.0f, -1.0f, 0.0f } },{ { -1.0f, -1.0f, 0.0f } },{ { -1.0f, 1.0f, 0.0f } } };
 //     const std::uint32_t   draw_count = 5;
 //
 //     data.vertices = builder.create<buffer_resource>("Test Vertices", buffer_description{
 //       vk::BufferCreateFlagBits(0),
 //       vertices.size() * sizeof(vertex_t),
 //       vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
 //       vk::SharingMode::eExclusive,
 //       nullptr,
 //       vk::MemoryPropertyFlagBits::eDeviceLocal });
 //
 //     data.pipeline = builder.create<pipeline_resource>("Test Pipeline", pipeline_description{
 //       std::string("test.vs"), test_vertex_shader  ,
 //       std::string("test.fs"), test_fragment_shader,
 //       vk::PrimitiveTopology::eTriangleStrip,
 //       vulkan_context().window_swapchains[0].render_pass // Test task applies to the first window.
 //       });
 //     
 // const vk::StencilOpState stencil_op_state(
 //   vk::StencilOp::eKeep           , 
 //   vk::StencilOp::eKeep           , 
 //   vk::StencilOp::eKeep           , 
 //   vk::CompareOp::eAlways         , 
 //   0                              , 
 //   0                              , 
 //   0                              );
 // const vk::PipelineColorBlendAttachmentState color_blend_attachment_state(
 //   false                          , 
 //   vk::BlendFactor::eZero         , 
 //   vk::BlendFactor::eZero         , 
 //   vk::BlendOp::eAdd              , 
 //   vk::BlendFactor::eZero         , 
 //   vk::BlendFactor::eZero         , 
 //   vk::BlendOp::eAdd              , 
 //   vk::ColorComponentFlagBits::eR | 
 //   vk::ColorComponentFlagBits::eG | 
 //   vk::ColorComponentFlagBits::eB | 
 //   vk::ColorComponentFlagBits::eA );
 //
 //     mak::vulkan_context().logical_device->createGraphicsPipeline(
 //       mak::vulkan_context().logical_device->createPipelineCache(0, nullptr),
 //       {},
 //   {
 //     vkhlf::PipelineShaderStageCreateInfo(
 //       vk::ShaderStageFlagBits::eVertex  ,
 //       mak::vulkan_context().logical_device->createShaderModule(vkhlf::compileGLSLToSPIRV(vk::ShaderStageFlagBits::eVertex  , description.vertex_shader_source)),
 //       description.vertex_shader_name),
 //       vkhlf::PipelineShaderStageCreateInfo(
 //         vk::ShaderStageFlagBits::eFragment,
 //         mak::vulkan_context().logical_device->createShaderModule(vkhlf::compileGLSLToSPIRV(vk::ShaderStageFlagBits::eFragment, description.fragment_shader_source)),
 //         description.fragment_shader_name)
 //   },
 //       vkhlf::PipelineVertexInputStateCreateInfo(vk::VertexInputBindingDescription(0, sizeof(vertex_t), vk::VertexInputRate::eVertex),
 //         {
 //           vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(vertex_t, vertex))
 //         }),
 //       vk::PipelineInputAssemblyStateCreateInfo({}, description.primitive_topology),
 //       nullptr,
 //       vkhlf::PipelineViewportStateCreateInfo({}, {}),
 //       vk::PipelineRasterizationStateCreateInfo({}, true, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise),
 //       vkhlf::PipelineMultisampleStateCreateInfo(vk::SampleCountFlagBits::e1, false, 0.0f, nullptr, false, false),
 //       vk::PipelineDepthStencilStateCreateInfo({}, true, true, vk::CompareOp::eLessOrEqual, false, false, stencil_op_state, stencil_op_state),
 //       vkhlf::PipelineColorBlendStateCreateInfo(false, vk::LogicOp::eNoOp, color_blend_attachment_state, { 1.0f, 1.0f, 1.0f, 1.0f }),
 //       vkhlf::PipelineDynamicStateCreateInfo({ vk::DynamicState::eViewport, vk::DynamicState::eScissor }),
 //       mak::vulkan_context().logical_device->createPipelineLayout(mak::vulkan_context().logical_device->createDescriptorSetLayout(std::vector<vkhlf::DescriptorSetLayoutBinding>()), nullptr),
 //       description.render_pass)
 //
 //       return std::make_unique<vkhlf::Buffer>(
 //         mak::vulkan_context().logical_device,
 //         description.flags,
 //         description.size,
 //         description.usage,
 //         description.sharing_mode,
 //         description.queue_family_indices,
 //         description.memory_property_flags,
 //         mak::vulkan_context().buffer_allocator,
 //         nullptr);
 //
 //     auto  command_buffer = vulkan_context().command_pool->allocateCommandBuffer();
 //     for(auto& window_swapchain : vulkan_context().window_swapchains)
 //     {
 //       command_buffer->begin           ();
 //       data.vertices->actual           ()->update<vertex_t>(0, {std::uint32_t(vertices.size()), vertices.data()}, command_buffer);
 //       command_buffer->beginRenderPass (
 //         window_swapchain.render_pass, 
 //         window_swapchain.swapchain->getFramebuffer(), 
 //         vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()), 
 //         {
 //           vk::ClearValue(std::array<float, 4>{0.1f, 0.1f, 0.1f}), 
 //           vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))
 //         }, 
 //         vk::SubpassContents::eInline);
 //       command_buffer->bindPipeline    (vk::PipelineBindPoint::eGraphics, *data.pipeline->actual());
 //       command_buffer->bindVertexBuffer(0, data.vertices->actual().get(), 0);
 //       command_buffer->setViewport     (0, vk::Viewport(
 //         0.0f, 
 //         0.0f, 
 //         static_cast<float>(window_swapchain.swapchain->getExtent().width ), 
 //         static_cast<float>(window_swapchain.swapchain->getExtent().height), 
 //         0.0f, 
 //         1.0f));
 //       command_buffer->setScissor      (0, vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()));
 //       command_buffer->draw            (std::uint32_t(draw_count), 1, 0, 0);
 //       command_buffer->endRenderPass   ();
 //       command_buffer->end             ();
 //       vulkan_context().graphics_queue->submit  (vkhlf::SubmitInfo{
 //         {window_swapchain.swapchain->getPresentSemaphore()}, 
 //         {vk::PipelineStageFlagBits::eColorAttachmentOutput}, 
 //         command_buffer, 
 //         vulkan_context().render_complete_semaphore});
 //     }
    });
}
fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph)
{
  return framegraph->add_render_task<present_task_data>(
    "Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const present_task_data& data)
    {
      context().present_window_swapchains();
    });
}
}
}
