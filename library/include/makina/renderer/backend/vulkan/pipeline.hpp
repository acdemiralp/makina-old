#ifndef MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_

#include <memory>

#include <fg/resource.hpp>
#include <vkhlf/Device.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT pipeline_description
{

};

using pipeline_resource = fg::resource<pipeline_description, vkhlf::Pipeline>;
}

template<>
inline std::unique_ptr<vkhlf::Buffer> fg::realize(const mak::pipeline_description& description)
{
  return nullptr;
}

#endif