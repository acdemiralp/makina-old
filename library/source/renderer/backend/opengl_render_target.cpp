#include <makina/renderer/backend/opengl_render_target.hpp>

#include <stdexcept>

namespace mak
{
opengl_render_target::opengl_render_target(const std::array<GLsizei, 2>& size, const GLenum color_format, const GLenum depth_format) : gl::framebuffer()
{
  color_texture_.set_storage(0, color_format, size[0], size[1]);
  depth_texture_.set_storage(0, depth_format, size[0], size[1]);
  attach_texture(GL_COLOR_ATTACHMENT0, color_texture_, 0);
  attach_texture(GL_DEPTH_ATTACHMENT , depth_texture_, 0);
  if (!is_complete())
    throw std::runtime_error("The render target framebuffer is incomplete!");
}
}
