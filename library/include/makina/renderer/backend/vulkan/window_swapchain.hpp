#ifndef MAKINA_RENDERER_BACKEND_VULKAN_WINDOW_SWAPCHAIN_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_WINDOW_SWAPCHAIN_HPP_

#include <memory>

#include <di/systems/display/vulkan_window.hpp>
#include <vkhlf/FramebufferSwapchain.h>
#include <vkhlf/Surface.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT window_swapchain
{
  di::vulkan_window*                           window    = nullptr;
  std::shared_ptr<vkhlf::Surface>              surface   = nullptr;
  std::shared_ptr<vkhlf::FramebufferSwapchain> swapchain = nullptr;
};
}

#endif