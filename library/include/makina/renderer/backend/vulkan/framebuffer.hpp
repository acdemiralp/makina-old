#ifndef MAKINA_RENDERER_BACKEND_VULKAN_FRAMEBUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_FRAMEBUFFER_HPP_

#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Allocator.h>
#include <vkhlf/Framebuffer.h>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT framebuffer_description
{
  std::shared_ptr<vkhlf::RenderPass>                      render_pass = nullptr ;
  vk::ArrayProxy<const std::shared_ptr<vkhlf::ImageView>> attachments = nullptr ;
  std::array<std::uint32_t, 2>                            size        = {1u, 1u};
  std::uint32_t                                           layers      = 1       ;
};

using framebuffer_resource = fg::resource<framebuffer_description, std::shared_ptr<vkhlf::Framebuffer>>;
}
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Framebuffer>> fg::realize(const mak::vulkan::framebuffer_description& description)
{
  return std::make_unique<std::shared_ptr<vkhlf::Framebuffer>>(mak::vulkan::context().logical_device->createFramebuffer(
    description.render_pass,
    description.attachments,
   {description.size[0]    , 
    description.size[1]}   ,
    description.layers     ,
    nullptr                )); 
}

#endif