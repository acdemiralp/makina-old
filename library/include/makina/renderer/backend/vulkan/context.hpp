#ifndef MAKINA_RENDERER_BACKEND_VULKAN_CONTEXT_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_CONTEXT_HPP_

#include <memory>
#include <vector>

#include <di/systems/display/vulkan_window.hpp>
#include <vkhlf/CommandPool.h>
#include <vkhlf/DebugReportCallback.h>
#include <vkhlf/DescriptorPool.h>
#include <vkhlf/DeviceMemoryAllocator.h>
#include <vkhlf/Device.h>
#include <vkhlf/Instance.h>
#include <vkhlf/PhysicalDevice.h>
#include <vkhlf/Queue.h>
#include <vkhlf/Semaphore.h>

#include <makina/renderer/backend/vulkan/window_swapchain.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT _context
{
  _context           ();
  _context           (const _context&  that) = delete ;
  _context           (      _context&& temp) = default;
  virtual ~_context  ()                      = default;
  _context& operator=(const _context&  that) = delete ;
  _context& operator=(      _context&& temp) = default;

  void create_window_swapchains (const std::vector<di::vulkan_window*>& windows);
  void present_window_swapchains();

  std::shared_ptr<vkhlf::Instance>              instance                     ;
  std::shared_ptr<vkhlf::DebugReportCallback>   debug_callback               ;
  std::shared_ptr<vkhlf::PhysicalDevice>        physical_device              ;
  std::shared_ptr<vkhlf::Device>                logical_device               ;
  std::shared_ptr<vkhlf::Queue>                 graphics_queue               ;
  std::shared_ptr<vkhlf::Semaphore>             render_complete_semaphore    ;
  std::shared_ptr<vkhlf::DeviceMemoryAllocator> buffer_allocator             ;
  std::shared_ptr<vkhlf::DeviceMemoryAllocator> image_allocator              ;
  std::shared_ptr<vkhlf::DescriptorPool>        descriptor_pool              ;
  std::shared_ptr<vkhlf::CommandPool>           command_pool                 ;
  std::vector<window_swapchain>                 window_swapchains            ;
};

MAKINA_EXPORT _context& context();
}
}

#endif