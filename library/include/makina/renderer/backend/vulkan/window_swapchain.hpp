#ifndef MAKINA_RENDERER_BACKEND_VULKAN_WINDOW_SWAPCHAIN_HPP
#define MAKINA_RENDERER_BACKEND_VULKAN_WINDOW_SWAPCHAIN_HPP

#include <cstdint>
#include <memory>

#include <vkhlf/Framebuffer.h>
#include <vkhlf/FramebufferSwapchain.h>
#include <vkhlf/RenderPass.h>
#include <vkhlf/Surface.h>
#include <di/systems/display/vulkan_window.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT window_swapchain
{
  di::vulkan_window*                           window               = nullptr               ;
  std::shared_ptr<vkhlf::Surface>              surface              = nullptr               ;
  std::shared_ptr<vkhlf::RenderPass>           render_pass          = nullptr               ;
  std::shared_ptr<vkhlf::FramebufferSwapchain> swapchain            = nullptr               ;
  std::shared_ptr<vkhlf::Framebuffer>          framebuffer          = nullptr               ;
  std::uint32_t                                queue_family_index   = 0                     ;
  vk::Format                                   color_format         = vk::Format::eUndefined;
  vk::Format                                   depth_format         = vk::Format::eUndefined;
};
}
}

#endif