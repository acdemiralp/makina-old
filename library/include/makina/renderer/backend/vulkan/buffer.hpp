#ifndef MAKINA_RENDERER_BACKEND_VULKAN_BUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_BUFFER_HPP_

#include <cstdint>
#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Buffer.h>

#include <makina/renderer/backend/vulkan/vulkan_context.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT buffer_description
{
  vk::BufferCreateFlags               flags                ;
  vk::DeviceSize                      size                 ;
  vk::BufferUsageFlags                usage                ;
  vk::SharingMode                     sharing_mode         ;
  vk::ArrayProxy<const std::uint32_t> queue_family_indices = nullptr;
  vk::MemoryPropertyFlags             memory_property_flags;
};

using buffer_resource = fg::resource<buffer_description, vkhlf::Buffer>;
}

template<>
inline std::unique_ptr<vkhlf::Buffer> fg::realize(const mak::buffer_description& description)
{
  return std::make_unique<vkhlf::Buffer>(
    mak::vulkan_context().logical_device  ,
    description.flags                     ,
    description.size                      ,
    description.usage                     ,
    description.sharing_mode              ,
    description.queue_family_indices      ,
    description.memory_property_flags     ,
    mak::vulkan_context().buffer_allocator,
    nullptr                               );
}

#endif