#ifndef MAKINA_RENDERER_BACKEND_OPENGL_FRAMEBUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_FRAMEBUFFER_HPP_

#include <array>

#include <fg/resource.hpp>
#include <gl/framebuffer.hpp>
#include <gl/texture.hpp>

#include <makina/export.hpp>

namespace mak
{
// mak::framebuffer is a gl::framebuffer with in-built color and depth textures.
class MAKINA_EXPORT framebuffer : public gl::framebuffer
{
public:
  struct MAKINA_EXPORT description
  {
    std::array<GLsizei, 2> size;
    GLenum                 color_format = GL_RGBA;
    GLenum                 depth_format = GL_DEPTH_COMPONENT32;
  };

  framebuffer           (); // Default framebuffer constructor.
  framebuffer           (const std::array<GLsizei, 2>& size, const GLenum color_format = GL_RGBA, const GLenum depth_format = GL_DEPTH_COMPONENT24);
  framebuffer           (const framebuffer&  that) = delete ;
  framebuffer           (      framebuffer&& temp) = default;
  virtual ~framebuffer()                           = default;
  framebuffer& operator=(const framebuffer&  that) = delete ;
  framebuffer& operator=(      framebuffer&& temp) = default;

protected:
  gl::texture_2d color_texture_;
  gl::texture_2d depth_texture_;
};

MAKINA_EXPORT framebuffer* default_framebuffer();

using framebuffer_resource = fg::resource<framebuffer::description, framebuffer>;
}

template<>
inline std::unique_ptr<mak::framebuffer> fg::realize(const mak::framebuffer::description& description)
{
  return std::make_unique<mak::framebuffer>(description.size, description.color_format, description.depth_format);
}

#endif