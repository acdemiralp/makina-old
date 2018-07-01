#ifndef MAKINA_RENDERER_BACKEND_VULKAN_SAMPLER_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_SAMPLER_HPP_

#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Sampler.h>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT sampler_description
{
  // Rearrangement of members is for convenience.
  vk::Filter                            min_filter         = vk::Filter::eLinear;
  vk::Filter                            mag_filter         = vk::Filter::eLinear;
  vk::SamplerMipmapMode                 mipmap_mode        = vk::SamplerMipmapMode::eLinear;
  std::array<vk::SamplerAddressMode, 3> address_mode       = {vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat};
  std::array<float, 2>                  lod_range          {0.0f, 1000.0f};
  float                                 mip_lod_bias       = 0.0f ;
  bool                                  enable_anisotropy  = false;
  float                                 maximum_anisotropy = 16   ;
  bool                                  enable_compare     = false;
  vk::CompareOp                         compare_op         = vk::CompareOp::eLess;
  vk::BorderColor                       border_color       = vk::BorderColor::eFloatOpaqueBlack;
  bool                                  unnormalized       = false;
};

using sampler_resource = fg::resource<sampler_description, std::shared_ptr<vkhlf::Sampler>>;
}
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Sampler>> fg::realize(const mak::vulkan::sampler_description& description)
{
  return std::make_unique<std::shared_ptr<vkhlf::Sampler>>(
    mak::vulkan::context().logical_device->createSampler(
    description.mag_filter                ,
    description.min_filter                ,
    description.mipmap_mode               ,
    description.address_mode[0]           ,
    description.address_mode[1]           ,
    description.address_mode[2]           ,
    description.mip_lod_bias              ,
    description.enable_anisotropy         ,
    description.maximum_anisotropy        ,
    description.enable_compare            ,
    description.compare_op                ,
    description.lod_range[0]              ,
    description.lod_range[1]              ,
    description.border_color              ,
    description.unnormalized              ,
    nullptr                               ));
}

#endif