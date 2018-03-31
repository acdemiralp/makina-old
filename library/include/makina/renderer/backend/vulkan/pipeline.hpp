#ifndef MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_

#include <memory>
#include <string>

#include <fg/resource.hpp>
#include <glm/vec3.hpp>
#include <vkhlf/Pipeline.h>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT pipeline_description
{
  std::string                                      vertex_shader_name    ;
  std::string                                      vertex_shader_source  ;
  std::string                                      fragment_shader_name  ;
  std::string                                      fragment_shader_source;
  vk::PrimitiveTopology                            primitive_topology    = vk::PrimitiveTopology::eTriangleList;
  std::shared_ptr<vkhlf::RenderPass>               render_pass           = nullptr                             ;
  std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;
  std::uint32_t                                    attribute_stride      ;
};

using pipeline_resource = fg::resource<pipeline_description, std::shared_ptr<vkhlf::Pipeline>>;
}
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Pipeline>> fg::realize(const mak::vulkan::pipeline_description& description)
{
  const vk::StencilOpState stencil_op_state(
    vk::StencilOp::eKeep           , 
    vk::StencilOp::eKeep           , 
    vk::StencilOp::eKeep           , 
    vk::CompareOp::eAlways         , 
    0                              , 
    0                              , 
    0                              );
  const vk::PipelineColorBlendAttachmentState color_blend_attachment_state(
    false                          , 
    vk::BlendFactor::eZero         , 
    vk::BlendFactor::eZero         , 
    vk::BlendOp::eAdd              , 
    vk::BlendFactor::eZero         , 
    vk::BlendFactor::eZero         , 
    vk::BlendOp::eAdd              , 
    vk::ColorComponentFlagBits::eR | 
    vk::ColorComponentFlagBits::eG | 
    vk::ColorComponentFlagBits::eB | 
    vk::ColorComponentFlagBits::eA );

  return std::make_unique<std::shared_ptr<vkhlf::Pipeline>>(mak::vulkan::context().logical_device->createGraphicsPipeline(
    mak::vulkan::context().logical_device->createPipelineCache(0, nullptr),
    {},
    {
      vkhlf::PipelineShaderStageCreateInfo(vk::ShaderStageFlagBits::eVertex  , mak::vulkan::context().logical_device->createShaderModule(vkhlf::compileGLSLToSPIRV(vk::ShaderStageFlagBits::eVertex  , description.vertex_shader_source  )), description.vertex_shader_name  ),
      vkhlf::PipelineShaderStageCreateInfo(vk::ShaderStageFlagBits::eFragment, mak::vulkan::context().logical_device->createShaderModule(vkhlf::compileGLSLToSPIRV(vk::ShaderStageFlagBits::eFragment, description.fragment_shader_source)), description.fragment_shader_name)
    },
    vkhlf::PipelineVertexInputStateCreateInfo(vk::VertexInputBindingDescription(0, description.attribute_stride, vk::VertexInputRate::eVertex), description.attribute_descriptions),
    vk::PipelineInputAssemblyStateCreateInfo ({}, description.primitive_topology),
    nullptr,
    vkhlf::PipelineViewportStateCreateInfo   ({}, {}),
    vk::PipelineRasterizationStateCreateInfo ({}, true, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise),
    vkhlf::PipelineMultisampleStateCreateInfo(vk::SampleCountFlagBits::e1, false, 0.0f, nullptr, false, false),
    vk::PipelineDepthStencilStateCreateInfo  ({}, true, true, vk::CompareOp::eLessOrEqual, false, false, stencil_op_state, stencil_op_state),
    vkhlf::PipelineColorBlendStateCreateInfo (false, vk::LogicOp::eNoOp, color_blend_attachment_state, {1.0f, 1.0f, 1.0f, 1.0f}),
    vkhlf::PipelineDynamicStateCreateInfo    ({vk::DynamicState::eViewport, vk::DynamicState::eScissor}),
    mak::vulkan::context().logical_device->createPipelineLayout(mak::vulkan::context().logical_device->createDescriptorSetLayout(std::vector<vkhlf::DescriptorSetLayoutBinding>()), nullptr),
    description.render_pass));
}

#endif