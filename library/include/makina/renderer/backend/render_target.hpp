#ifndef MAKINA_RENDERER_BACKEND_RENDER_TARGET_HPP_
#define MAKINA_RENDERER_BACKEND_RENDER_TARGET_HPP_

#include <array>

#include <gl/framebuffer.hpp>
#include <gl/texture.hpp>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT render_target : public gl::framebuffer
{
public:
  explicit render_target  (const std::array<GLsizei, 2>& size, const GLenum color_format = GL_RGBA, const GLenum depth_format = GL_DEPTH_COMPONENT24);
  render_target           (const render_target&  that) = delete ;
  render_target           (      render_target&& temp) = default;
  virtual ~render_target  ()                           = default;
  render_target& operator=(const render_target&  that) = delete ;
  render_target& operator=(      render_target&& temp) = default;

protected:
  gl::texture_2d color_texture_;
  gl::texture_2d depth_texture_;
};
}

#endif