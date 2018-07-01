#ifndef MAKINA_RENDERER_BACKEND_OPENGL_FRAMEBUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_FRAMEBUFFER_HPP_

#include <array>
#include <memory>

#include <di/systems/display/window.hpp>
#include <fg/resource.hpp>
#include <gl/framebuffer.hpp>
#include <gl/texture.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
// A gl::framebuffer with in-built color and depth textures.
class MAKINA_EXPORT framebuffer : public gl::framebuffer
{
public:
  struct MAKINA_EXPORT description
  {
    std::array<GLsizei, 2> size;
    GLenum                 color_format = GL_RGBA8;
    GLenum                 depth_format = GL_DEPTH_COMPONENT24;
  };

  explicit framebuffer  (di::window* window); // Default framebuffer constructor.
  explicit framebuffer  (const description&  description);
  framebuffer           (const framebuffer&  that) = delete ;
  framebuffer           (      framebuffer&& temp) = default;
  virtual ~framebuffer()                           = default;
  framebuffer& operator=(const framebuffer&  that) = delete ;
  framebuffer& operator=(      framebuffer&& temp) = default;
  
  gl::texture_2d* color_texture();
  gl::texture_2d* depth_texture();

  bool is_default() const;

protected:
  gl::texture_2d color_texture_;
  gl::texture_2d depth_texture_;
};

MAKINA_EXPORT framebuffer* default_framebuffer(di::window* window);

using framebuffer_resource = fg::resource<framebuffer::description, framebuffer>;
}
}

template<>
inline std::unique_ptr<mak::opengl::framebuffer> fg::realize(const mak::opengl::framebuffer::description& description)
{
  return std::make_unique<mak::opengl::framebuffer>(description);
}

#endif