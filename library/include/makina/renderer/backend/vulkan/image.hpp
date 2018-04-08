#ifndef MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_IMAGE_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include <fg/resource.hpp>
#include <vkhlf/Image.h>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT image_description
{
  // Rearrangement of members is for convenience.
  vk::ImageType                 type                  ;
  vk::Extent3D                  size                  ;
  vk::Format                    format                ;
  vk::ImageUsageFlags           usage_flags           = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
  vk::ImageCreateFlags          create_flags          = vk::ImageCreateFlagBits::eSparseBinding;
  vk::ImageLayout               initial_layout        = vk::ImageLayout::eUndefined;
  std::uint32_t                 mip_levels            = 1;
  std::uint32_t                 array_layers          = 1;
  vk::SampleCountFlagBits       samples               = vk::SampleCountFlagBits::e1;
  vk::ImageTiling               tiling                = vk::ImageTiling::eOptimal;
  vk::SharingMode               sharing_mode          = vk::SharingMode::eExclusive;
  std::vector<std::uint32_t>    queue_family_indices  {0};
  vk::MemoryPropertyFlags       memory_property_flags = vk::MemoryPropertyFlagBits::eDeviceLocal;
};

using image_resource = fg::resource<image_description, std::shared_ptr<vkhlf::Image>>;
}
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Image>> fg::realize(const mak::vulkan::image_description& description)
{
  return std::make_unique<std::shared_ptr<vkhlf::Image>>(mak::vulkan::context().logical_device->createImage(
    description.create_flags              ,
    description.type                      ,
    description.format                    ,
    description.size                      ,
    description.mip_levels                ,
    description.array_layers              ,
    description.samples                   ,
    description.tiling                    ,
    description.usage_flags               ,
    description.sharing_mode              ,
    description.queue_family_indices      ,
    description.initial_layout            ,
    description.memory_property_flags     ,
    mak::vulkan::context().image_allocator,
    nullptr                               ));
}

#endif