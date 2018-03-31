#ifndef MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_PIPELINE_HPP_

#include <memory>
#include <string>

#include <fg/resource.hpp>
#include <vkhlf/Pipeline.h>

#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT pipeline_description
{

};

using pipeline_resource = fg::resource<pipeline_description, vkhlf::Pipeline>;
}
}

template<>
inline std::unique_ptr<vkhlf::Pipeline> fg::realize(const mak::vulkan::pipeline_description& description)
{
  return nullptr;
}

#endif