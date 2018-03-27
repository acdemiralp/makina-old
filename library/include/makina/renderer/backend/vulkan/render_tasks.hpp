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

#include <makina/aspects/singleton.hpp>
#include <makina/config.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT window_swapchain
{
  di::vulkan_window*                           window   ;
  std::shared_ptr<vkhlf::Surface>              surface  ;
  std::shared_ptr<vkhlf::FramebufferSwapchain> swapchain;
};

struct MAKINA_EXPORT vulkan_context : public singleton<vulkan_context>
{
  vulkan_context           ()
  {
    const auto               name       = typeid(*this).name();
    const auto               version    = VK_MAKE_VERSION(1, 0, 0);
    std::vector<std::string> layers     {"VK_LAYER_LUNARG_standard_validation"};
    std::vector<std::string> extensions {"VK_KHR_surface"};
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    extensions.push_back("VK_KHR_android_surface");
#endif
#if defined(VK_USE_PLATFORM_MIR_KHR)
    extensions.push_back("VK_KHR_mir_surface");
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    extensions.push_back("VK_KHR_wayland_surface");
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    extensions.push_back("VK_KHR_win32_surface");
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    extensions.push_back("VK_KHR_xcb_surface");
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    extensions.push_back("VK_KHR_xlib_surface");
#endif

    instance = vkhlf::Instance::create(name, version, layers, extensions, nullptr);

#ifndef NDEBUG
    debug_callback = instance->createDebugReportCallback(
      vk::DebugReportFlagsEXT(
        vk::DebugReportFlagBitsEXT::eWarning            |
        vk::DebugReportFlagBitsEXT::ePerformanceWarning |
        vk::DebugReportFlagBitsEXT::eError              |
        vk::DebugReportFlagBitsEXT::eDebug              ), &vkhlf::debugReportCallback);
#endif
  }
  vulkan_context           (const vulkan_context&  that) = default;
  vulkan_context           (      vulkan_context&& temp) = default;
  virtual ~vulkan_context  ()                            = default;
  vulkan_context& operator=(const vulkan_context&  that) = default;
  vulkan_context& operator=(      vulkan_context&& temp) = default;

  void present()
  {
    for (auto& window_swapchain : window_swapchains)
    {
      window_swapchain.swapchain->present         (graphics_queue, render_complete_semaphore);
      window_swapchain.swapchain->acquireNextFrame();
    }
  }

  std::shared_ptr<vkhlf::Instance>            instance                 ;
  std::shared_ptr<vkhlf::DebugReportCallback> debug_callback           ;
  std::shared_ptr<vkhlf::PhysicalDevice>      physical_device          ;
  std::shared_ptr<vkhlf::Device>              logical_device           ;
  std::shared_ptr<vkhlf::Queue>               graphics_queue           ;
  std::shared_ptr<vkhlf::Semaphore>           render_complete_semaphore;
  std::uint32_t                               queue_family_index       = 0;
  vk::Format                                  color_format             = vk::Format::eUndefined;
  vk::Format                                  depth_format             = vk::Format::eUndefined;
  std::vector<window_swapchain>               window_swapchains        ;
};
}

#endif