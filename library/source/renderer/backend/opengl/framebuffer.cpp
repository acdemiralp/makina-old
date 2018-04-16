#include <makina/renderer/backend/opengl/framebuffer.hpp>

#include <stdexcept>

namespace mak
{
namespace opengl
{
framebuffer::framebuffer(di::opengl_window* window) : gl::framebuffer(0)
{
  auto size = window->size();
  color_texture_.set_storage(1, GL_RGBA8            , size[0], size[1]);
  depth_texture_.set_storage(1, GL_DEPTH_COMPONENT24, size[0], size[1]);
  window->on_resize.connect([&] (const std::array<std::size_t, 2>& size)
  {
    color_texture_ = gl::texture_2d();
    depth_texture_ = gl::texture_2d();
    color_texture_.set_storage(1, GL_RGBA8            , size[0], size[1]);
    depth_texture_.set_storage(1, GL_DEPTH_COMPONENT24, size[0], size[1]);
  });
}
framebuffer::framebuffer(const description& description)
{
  color_texture_.set_storage(1, description.color_format, description.size[0], description.size[1]);
  depth_texture_.set_storage(1, description.depth_format, description.size[0], description.size[1]);
  attach_texture(GL_COLOR_ATTACHMENT0, color_texture_, 0);
  attach_texture(GL_DEPTH_ATTACHMENT , depth_texture_, 0);
  if (!is_complete())
    throw std::runtime_error("The render target framebuffer is incomplete!");
}

gl::texture_2d* framebuffer::color_texture()
{
  return &color_texture_;
}
gl::texture_2d* framebuffer::depth_texture()
{
  return &depth_texture_;
}

bool framebuffer::is_default() const
{
  return id_ == 0;
}

framebuffer* default_framebuffer(di::opengl_window* window)
{
  static auto default_framebuffer = framebuffer(window);
  return &default_framebuffer;
}
}
}
