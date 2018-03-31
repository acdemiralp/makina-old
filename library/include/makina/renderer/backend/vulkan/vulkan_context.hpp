#ifndef MAKINA_RENDERER_BACKEND_VULKAN_VULKAN_CONTEXT_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_VULKAN_CONTEXT_HPP_

#include <memory>
#include <vector>

#include <di/systems/display/vulkan_window.hpp>
#include <vkhlf/CommandPool.h>
#include <vkhlf/DebugReportCallback.h>
#include <vkhlf/DeviceMemoryAllocator.h>
#include <vkhlf/Device.h>
#include <vkhlf/Instance.h>
#include <vkhlf/PhysicalDevice.h>
#include <vkhlf/Queue.h>
#include <vkhlf/Semaphore.h>

#include <makina/aspects/singleton.hpp>
#include <makina/renderer/backend/vulkan/window_swapchain.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT vulkan_context : public singleton<vulkan_context>
{
  vulkan_context           ();
  vulkan_context           (const vulkan_context&  that) = default;
  vulkan_context           (      vulkan_context&& temp) = default;
  virtual ~vulkan_context  ()                            = default;
  vulkan_context& operator=(const vulkan_context&  that) = default;
  vulkan_context& operator=(      vulkan_context&& temp) = default;

  void create_window_swapchains (const std::vector<di::vulkan_window*>& windows);
  void present_window_swapchains();

  std::shared_ptr<vkhlf::Instance>              instance                 ;
  std::shared_ptr<vkhlf::DebugReportCallback>   debug_callback           ;
  std::shared_ptr<vkhlf::PhysicalDevice>        physical_device          ;
  std::shared_ptr<vkhlf::Device>                logical_device           ;
  std::shared_ptr<vkhlf::Queue>                 graphics_queue           ;
  std::shared_ptr<vkhlf::Semaphore>             render_complete_semaphore;
  std::shared_ptr<vkhlf::DeviceMemoryAllocator> buffer_allocator         ;
  std::shared_ptr<vkhlf::DeviceMemoryAllocator> image_allocator          ;
  std::shared_ptr<vkhlf::CommandPool>           command_pool             ;
  std::vector<window_swapchain>                 window_swapchains        ;
};
}

#endif