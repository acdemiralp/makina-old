#ifndef MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include <fg/resource.hpp>
#include <vkhlf/Image.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT image_description
{
  std::shared_ptr<vkhlf::Device>                device                 ;
  vk::ImageCreateFlags                          create_flags           ;
  vk::ImageType                                 type                   ;
  vk::Format                                    format                 ;
  vk::Extent3D                                  extent                 ;
  std::uint32_t                                 mip_levels             ;
  std::uint32_t                                 array_layers           ;
  vk::SampleCountFlagBits                       samples                ;
  vk::ImageTiling                               tiling                 ;
  vk::ImageUsageFlags                           usage_flags            ;
  vk::SharingMode                               sharing_mode           ;
  std::vector<std::uint32_t>                    queue_family_indices   ;
  vk::ImageLayout                               initial_layout         ;
  vk::MemoryPropertyFlags                       memory_property_flags  ;
  std::shared_ptr<vkhlf::DeviceMemoryAllocator> device_memory_allocator;
  std::shared_ptr<vkhlf::Allocator>             image_allocator        ;
};

using image_resource = fg::resource<image_description, vkhlf::Image>;
}

template<>
inline std::unique_ptr<vkhlf::Image> fg::realize(const mak::image_description& description)
{
  return std::make_unique<vkhlf::Image>(
    description.device                 ,
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
    description.device_memory_allocator,
    description.image_allocator        );
}

#endif