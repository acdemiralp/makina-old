#include <makina/renderer/backend/vulkan/context.hpp>

#include <stdexcept>

namespace mak
{
namespace vulkan
{
_context::_context()
{
  const auto               name       = typeid(*this).name();
  const auto               version    = VK_MAKE_VERSION(1, 0, 0);
  std::vector<std::string> layers     {"VK_LAYER_LUNARG_standard_validation"};
  std::vector<std::string> extensions {"VK_KHR_surface"};

#if defined VK_USE_PLATFORM_ANDROID_KHR
  extensions.push_back("VK_KHR_android_surface");
#elif defined VK_USE_PLATFORM_MIR_KHR
  extensions.push_back("VK_KHR_mir_surface");
#elif defined VK_USE_PLATFORM_WAYLAND_KHR
  extensions.push_back("VK_KHR_wayland_surface");
#elif defined VK_USE_PLATFORM_WIN32_KHR
  extensions.push_back("VK_KHR_win32_surface");
#elif defined VK_USE_PLATFORM_XCB_KHR
  extensions.push_back("VK_KHR_xcb_surface");
#elif defined VK_USE_PLATFORM_XLIB_KHR
  extensions.push_back("VK_KHR_xlib_surface");
#endif

  instance       = vkhlf::Instance::create(name, version, layers, extensions, nullptr);
#ifndef NDEBUG
  debug_callback = instance->createDebugReportCallback(vk::DebugReportFlagsEXT(
    vk::DebugReportFlagBitsEXT::eWarning            |
    vk::DebugReportFlagBitsEXT::ePerformanceWarning |
    vk::DebugReportFlagBitsEXT::eError              |
    vk::DebugReportFlagBitsEXT::eDebug              ), 
    &vkhlf::debugReportCallback);
#endif

  for (size_t i = 0; i < instance->getPhysicalDeviceCount(); ++i)
  {
#if defined VK_USE_PLATFORM_ANDROID_KHR
    physical_device   = instance_->getPhysicalDevice(i);
    break;
#elif defined VK_USE_PLATFORM_MIR_KHR
    if (vkGetPhysicalDeviceMirPresentationSupportKHR    (static_cast<vk::PhysicalDevice>(*instance->getPhysicalDevice(i)), 0, sys_wm_info.info.mir.connection))
    {
      physical_device = instance->getPhysicalDevice(i);
      break;
    }
#elif defined VK_USE_PLATFORM_WAYLAND_KHR
    if (vkGetPhysicalDeviceWaylandPresentationSupportKHR(static_cast<vk::PhysicalDevice>(*instance->getPhysicalDevice(i)), 0, sys_wm_info.info.wl.display))
    {
      physical_device = instance->getPhysicalDevice(i);
      break;
    }
#elif defined VK_USE_PLATFORM_WIN32_KHR
    if (vkGetPhysicalDeviceWin32PresentationSupportKHR  (static_cast<vk::PhysicalDevice>(*instance->getPhysicalDevice(i)), 0))
    {
      physical_device = instance->getPhysicalDevice(i);
      break;
    }
#elif defined VK_USE_PLATFORM_XCB_KHR
    if (vkGetPhysicalDeviceXcbPresentationSupportKHR    (static_cast<vk::PhysicalDevice>(*instance->getPhysicalDevice(i)), 0, XGetXCBConnection(sys_wm_info.info.x11.display), XVisualIDFromVisual(DefaultVisual(sys_wm_info.info.x11.display, DefaultScreen(sys_wm_info.info.x11.display)))))
    {
      physical_device = instance->getPhysicalDevice(i);
      break;
    }
#elif defined VK_USE_PLATFORM_XLIB_KHR
    if (vkGetPhysicalDeviceXlibPresentationSupportKHR   (static_cast<vk::PhysicalDevice>(*instance->getPhysicalDevice(i)), 0, sys_wm_info.info.x11.display, XVisualIDFromVisual(DefaultVisual(sys_wm_info.info.x11.display, DefaultScreen(sys_wm_info.info.x11.display)))))
    {
      physical_device = instance->getPhysicalDevice(i);
      break;
    }
#endif
  }
  if  (!physical_device) 
    throw std::runtime_error("Failed to find a physical device with presentation support.");

  logical_device            = physical_device->createDevice     (vkhlf::DeviceQueueCreateInfo(0 /* queue family index */, 0.0f /* queue priorities */), nullptr, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
  graphics_queue            = logical_device ->getQueue         (0 /* queue family index */, 0 /* queue index */);
  render_complete_semaphore = logical_device ->createSemaphore  ();
  command_pool              = logical_device ->createCommandPool(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  buffer_allocator          = std::make_shared<vkhlf::DeviceMemoryAllocator>(logical_device, vk::DeviceSize(64  * 1024), nullptr);
  image_allocator           = std::make_shared<vkhlf::DeviceMemoryAllocator>(logical_device, vk::DeviceSize(128 * 1024), nullptr);
}

void _context::create_window_swapchains (const std::vector<di::vulkan_window*>& windows)
{
  for (auto& window : windows)
  {
    window_swapchains.emplace_back();
    auto& window_swapchain = window_swapchains.back();
    window_swapchain.window = window;

    auto driver_data = window->driver_data();
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    window_swapchain.surface = instance->createSurface(std::get<0>(driver_data));
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    window_swapchain.surface = instance->createSurface(std::get<0>(driver_data), std::get<1>(driver_data));
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    window_swapchain.surface = instance->createSurface(std::get<0>(driver_data), std::get<1>(driver_data));
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
    window_swapchain.surface = instance->createSurface(std::get<2>(driver_data), std::get<0>(driver_data));
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    window_swapchain.surface = instance->createSurface(XGetXCBConnection(std::get<0>(driver_data)), std::get<1>(driver_data));
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    window_swapchain.surface = instance->createSurface(std::get<0>(driver_data), std::get<1>(driver_data));
#endif
      
    auto queue_family_indices           = getGraphicsPresentQueueFamilyIndices(physical_device, window_swapchain.surface);
    auto surface_formats                = physical_device->getSurfaceFormats(window_swapchain.surface);
    window_swapchain.queue_family_index = queue_family_indices[0];
    window_swapchain.color_format       = surface_formats.size() == 1 && surface_formats[0].format == vk::Format::eUndefined ? vk::Format::eB8G8R8A8Unorm : surface_formats[0].format;
    window_swapchain.depth_format       = vk::Format::eD24UnormS8Uint;
    vk::AttachmentReference color_reference(0, vk::ImageLayout::eColorAttachmentOptimal       );
    vk::AttachmentReference depth_reference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
    window_swapchain.render_pass        = logical_device->createRenderPass({
      vk::AttachmentDescription(
        {}                              , 
        window_swapchain.color_format   ,
        vk::SampleCountFlagBits::e1     ,
        vk::AttachmentLoadOp::eClear    , 
        vk::AttachmentStoreOp::eStore   ,
        vk::AttachmentLoadOp::eDontCare , 
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined     , 
        vk::ImageLayout::ePresentSrcKHR ),
      vk::AttachmentDescription(
        {}                              , 
        window_swapchain.depth_format   ,
        vk::SampleCountFlagBits::e1     , 
        vk::AttachmentLoadOp::eClear    , 
        vk::AttachmentStoreOp::eStore   ,
        vk::AttachmentLoadOp::eDontCare , 
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined     ,
        vk::ImageLayout::eDepthStencilAttachmentOptimal)
      },
      vk::SubpassDescription(
        {}                              , 
        vk::PipelineBindPoint::eGraphics, 
        0                               , 
        nullptr                         , 
        1                               ,
        &color_reference                , 
        nullptr                         ,
        &depth_reference                , 
        0                               , 
        nullptr), nullptr);

    window_swapchain.swapchain = std::make_shared<vkhlf::FramebufferSwapchain>(
      logical_device               ,
      window_swapchain.surface     ,
      window_swapchain.color_format,
      window_swapchain.depth_format,
      window_swapchain.render_pass );
    window_swapchain.window->on_resize.connect([&] (const std::array<std::size_t, 2>& size)
    {
      window_swapchain.swapchain = std::make_shared<vkhlf::FramebufferSwapchain>(
        logical_device               ,
        window_swapchain.surface     ,
        window_swapchain.color_format,
        window_swapchain.depth_format,
        window_swapchain.render_pass );
    });
  }
}
void _context::present_window_swapchains()
{
  for (auto& window_swapchain : window_swapchains)
  {
    window_swapchain.swapchain->present         (graphics_queue, render_complete_semaphore);
    window_swapchain.swapchain->acquireNextFrame();
  }
}

_context& context()
{
  static _context context;
  return context;
}
}
}