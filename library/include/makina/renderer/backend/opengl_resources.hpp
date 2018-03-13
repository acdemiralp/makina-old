#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_

#include <array>
#include <stdexcept>

#include <fg/framegraph.hpp>
#include <gl/all.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT buffer_description
{
  GLsizeiptr                 size             ;
  GLenum                     type             = GL_ARRAY_BUFFER;
};
struct MAKINA_EXPORT sampler_description
{
  std::array<GLenum, 3>      wrap             = {GL_REPEAT, GL_REPEAT, GL_REPEAT};
  GLenum                     min_filter       = GL_NEAREST;
  GLenum                     mag_filter       = GL_NEAREST;
};
struct MAKINA_EXPORT texture_description
{
  std::array<GLsizei, 3>     size             ;
  GLenum                     format           = GL_RGBA;
  GLsizei                    levels           = 0      ;
};
struct MAKINA_EXPORT framebuffer_description
{
  std::array<GLsizei, 2>     size             ;
  std::array<GLfloat, 4>     clear_color      = {0.0f, 0.0f, 0.0f, 1.0f};
  GLenum                     color_format     = GL_RGBA;
  GLenum                     depth_format     = GL_DEPTH_COMPONENT32;
};

using buffer_resource      = fg::resource<buffer_description     , gl::buffer     >;
using sampler_resource     = fg::resource<sampler_description    , gl::sampler    >;
using texture_1d_resource  = fg::resource<texture_description    , gl::texture_1d >;
using texture_2d_resource  = fg::resource<texture_description    , gl::texture_2d >;
using texture_3d_resource  = fg::resource<texture_description    , gl::texture_3d >;
using framebuffer_resource = fg::resource<framebuffer_description, gl::framebuffer>;
}

template<>
inline std::unique_ptr<gl::buffer>      fg::realize(const mak::buffer_description&      description)
{
  auto   buffer = std::make_unique<gl::buffer>();
  buffer->set_data_immutable(description.size);
  buffer->bind              (description.type);
  buffer->unbind            (description.type);
  return buffer;
}
template<>
inline std::unique_ptr<gl::sampler>     fg::realize(const mak::sampler_description&     description)
{
  auto   sampler = std::make_unique<gl::sampler>();
  sampler->set_wrap_s    (description.wrap[0]);
  sampler->set_wrap_t    (description.wrap[1]);
  sampler->set_wrap_r    (description.wrap[2]);
  sampler->set_min_filter(description.min_filter);
  sampler->set_mag_filter(description.mag_filter);
  return sampler;
}
template<>
inline std::unique_ptr<gl::texture_1d>  fg::realize(const mak::texture_description&     description)
{
  auto   texture = std::make_unique<gl::texture_1d>();
  texture->set_storage(description.levels, description.format, description.size[0]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_2d>  fg::realize(const mak::texture_description&     description)
{
  auto   texture = std::make_unique<gl::texture_2d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_3d>  fg::realize(const mak::texture_description&     description)
{
  auto   texture = std::make_unique<gl::texture_3d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1], description.size[2]);
  return texture;
}
template<>
inline std::unique_ptr<gl::framebuffer> fg::realize(const mak::framebuffer_description& description)
{
  auto framebuffer   = std::make_unique<gl::framebuffer>();
  auto color_texture = new gl::texture_2d(); // TODO: Prevent leak.
  auto depth_texture = new gl::texture_2d(); // TODO: Prevent leak.
  color_texture->set_storage   (0, description.color_format, description.size[0], description.size[1]);
  depth_texture->set_storage   (0, description.depth_format, description.size[0], description.size[1]);
  framebuffer  ->attach_texture(GL_COLOR_ATTACHMENT0, *color_texture, 0);
  framebuffer  ->attach_texture(GL_DEPTH_ATTACHMENT , *depth_texture, 0);
  if (!framebuffer->is_complete()) throw std::runtime_error("The realized framebuffer is incomplete!");
  return framebuffer;
}

#endif