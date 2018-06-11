#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_FRAMEBUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_FRAMEBUFFER_HPP_

#include <cstdint>

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT framebuffer_description
{
  ospcommon::vec2i     size     ; 
  OSPFrameBufferFormat format   = OSP_FB_SRGBA;
  std::int32_t         channels = OSP_FB_COLOR;
};
using framebuffer_resource = fg::resource<framebuffer_description, ::ospray::cpp::FrameBuffer>;
}
}

template<>
inline std::unique_ptr<ospray::cpp::FrameBuffer> fg::realize(const mak::ospray::framebuffer_description& description)
{
  return std::make_unique<ospray::cpp::FrameBuffer>(description.size, description.format, description.channels);
}

#endif