#ifndef MAKINA_RENDERER_BACKEND_VULKAN_BUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_BUFFER_HPP_

#include <cstdint>
#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Buffer.h>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT buffer_description
{
  // Rearrangement of members is for convenience.
  vk::DeviceSize                      size                  ;                        
  vk::BufferUsageFlags                usage_flags           ;
  vk::BufferCreateFlags               create_flags          = vk::BufferCreateFlagBits(0);
  vk::SharingMode                     sharing_mode          = vk::SharingMode::eExclusive;
  vk::ArrayProxy<const std::uint32_t> queue_family_indices  = nullptr;
  vk::MemoryPropertyFlags             memory_property_flags = vk::MemoryPropertyFlagBits::eDeviceLocal;
};

using buffer_resource = fg::resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>;
}
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Buffer>> fg::realize(const mak::vulkan::buffer_description& description)
{
  return std::make_unique<std::shared_ptr<vkhlf::Buffer>>(mak::vulkan::context().logical_device->createBuffer(
    description.create_flags               ,
    description.size                       ,
    description.usage_flags                ,
    description.sharing_mode               ,
    description.queue_family_indices       ,
    description.memory_property_flags      ,
    mak::vulkan::context().buffer_allocator,
    nullptr                                ));
}

#endif