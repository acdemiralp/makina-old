#ifndef MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include <fg/resource.hpp>
#include <vkhlf/Image.h>

#include <makina/renderer/backend/vulkan/vulkan_context.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT image_description
{
  vk::ImageCreateFlags      create_flags         ;
  vk::ImageType             type                 ;
  vk::Format                format               ;
  vk::Extent3D              extent               ;
  std::uint32_t             mip_levels           ;
  std::uint32_t             array_layers         ;
  vk::SampleCountFlagBits   samples              ;
  vk::ImageTiling           tiling               ;
  vk::ImageUsageFlags       usage_flags          ;
  vk::SharingMode           sharing_mode         ;
  std::vector<std::uint32_t>queue_family_indices ;
  vk::ImageLayout           initial_layout       ;
  vk::MemoryPropertyFlags   memory_property_flags;
};

using image_resource = fg::resource<image_description, std::shared_ptr<vkhlf::Image>>;
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Image>> fg::realize(const mak::image_description& description)
{
  return std::make_unique<std::shared_ptr<vkhlf::Image>>(mak::vulkan_context.logical_device->createImage(
    description.create_flags           ,
    description.type                   ,
    description.format                 ,
    description.extent                 ,
    description.mip_levels             ,
    description.array_layers           ,
    description.samples                ,
    description.tiling                 ,
    description.usage_flags            ,
    description.sharing_mode           ,
    description.queue_family_indices   ,
    description.initial_layout         ,
    description.memory_property_flags  ,
    mak::vulkan_context.image_allocator,
    nullptr                            ));
}

#endif