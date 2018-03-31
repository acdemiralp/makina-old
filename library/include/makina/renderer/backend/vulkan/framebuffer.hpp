#ifndef MAKINA_RENDERER_BACKEND_VULKAN_FRAMEBUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_FRAMEBUFFER_HPP_

#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Framebuffer.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT framebuffer_description
{

};

using framebuffer_resource = fg::resource<framebuffer_description, std::shared_ptr<vkhlf::Framebuffer>>;
}

template<>
inline std::unique_ptr<std::shared_ptr<vkhlf::Buffer>> fg::realize(const mak::framebuffer_description& description)
{
  return nullptr;
}

#endif