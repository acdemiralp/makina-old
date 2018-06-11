#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_TEXTURE_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_TEXTURE_HPP_

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT texture_description
{
  void*            data   ;
  osp::vec2i       size   ;
  OSPTextureFormat format = OSP_TEXTURE_RGBA8;
  bool             shared = false            ;
};

using texture_resource = fg::resource<texture_description, ::ospray::cpp::Texture2D>;
}
}

template<>
inline std::unique_ptr<ospray::cpp::Texture2D> fg::realize(const mak::ospray::texture_description& description)
{ 
  return std::make_unique<ospray::cpp::Texture2D>(ospNewTexture2D(description.size, description.format, description.data, description.shared ? OSP_TEXTURE_SHARED_BUFFER : 0));
}

#endif