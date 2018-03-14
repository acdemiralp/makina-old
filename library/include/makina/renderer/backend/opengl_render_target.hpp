#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TARGET_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TARGET_HPP_

#include <array>

#include <gl/framebuffer.hpp>
#include <gl/texture.hpp>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT opengl_render_target : public gl::framebuffer
{
public:
  explicit opengl_render_target  (); // Backbuffer constructor.
  explicit opengl_render_target  (const std::array<GLsizei, 2>& size, const GLenum color_format = GL_RGBA, const GLenum depth_format = GL_DEPTH_COMPONENT24);
  opengl_render_target           (const opengl_render_target&  that) = delete ;
  opengl_render_target           (      opengl_render_target&& temp) = default;
  virtual ~opengl_render_target  ()                           = default;
  opengl_render_target& operator=(const opengl_render_target&  that) = delete ;
  opengl_render_target& operator=(      opengl_render_target&& temp) = default;

protected:
  gl::texture_2d color_texture_;
  gl::texture_2d depth_texture_;
};
}

#endif