#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_

#include <di/systems/display/vulkan_window.hpp>
#include <vkhlf/DebugReportCallback.h>
#include <vkhlf/Device.h>
#include <vkhlf/FramebufferSwapchain.h>
#include <vkhlf/Instance.h>
#include <vkhlf/PhysicalDevice.h>
#include <vkhlf/Queue.h>
#include <vkhlf/Surface.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT vulkan_context
{
  std::shared_ptr<vkhlf::Instance>             instance       ;
  std::shared_ptr<vkhlf::DebugReportCallback>  debug_callback ;
  std::shared_ptr<vkhlf::PhysicalDevice>       physical_device;
  std::shared_ptr<vkhlf::Device>               logical_device ;
  std::shared_ptr<vkhlf::Queue>                graphics_queue ;
};

struct MAKINA_EXPORT window_swapchain
{
  di::vulkan_window*                           window         ;
  std::shared_ptr<vkhlf::Surface>              surface        ;
  std::unique_ptr<vkhlf::FramebufferSwapchain> swapchain      ;
};

void create_swapchains(const std::vector<di::window*> windows);
}

#endif