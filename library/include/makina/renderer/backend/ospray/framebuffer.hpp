#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_FRAMEBUFFER_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_FRAMEBUFFER_HPP

#include <cstdint>

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT framebuffer : ::ospray::cpp::FrameBuffer
{
  struct MAKINA_EXPORT description
  {
    ospcommon::vec2i     size     ; 
    OSPFrameBufferFormat format   = OSP_FB_SRGBA;
    std::int32_t         channels = OSP_FB_COLOR;
  };

  explicit framebuffer(const description& description) 
  : FrameBuffer(description.size, description.format, description.channels), size(description.size), format(description.format), channels(description.channels)
  {
    
  }
  framebuffer           (const framebuffer&  that) = default;
  framebuffer           (      framebuffer&& temp) = default;
  ~framebuffer          ()                         = default;
  framebuffer& operator=(const framebuffer&  that) = default;
  framebuffer& operator=(      framebuffer&& temp) = default;

  ospcommon::vec2i     size     ; 
  OSPFrameBufferFormat format   ;
  std::int32_t         channels ;
};

using framebuffer_resource = fg::resource<framebuffer::description, framebuffer>;
}
}

template<>
inline std::unique_ptr<mak::ospray::framebuffer> fg::realize(const mak::ospray::framebuffer::description& description)
{
  return std::make_unique<mak::ospray::framebuffer>(description);
}

#endif