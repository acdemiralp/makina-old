#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_

#include <array>

#include <fg/framegraph.hpp>
#include <gl/buffer.hpp>
#include <gl/sampler.hpp>
#include <gl/texture.hpp>

#include <makina/renderer/backend/opengl_render_target.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT buffer_description
{
  GLsizeiptr             size         ;
  GLenum                 type         = GL_ARRAY_BUFFER;
};
struct MAKINA_EXPORT texture_description
{
  std::array<GLsizei, 3> size         ;
  GLenum                 format       = GL_RGBA;
  GLsizei                levels       = 0      ;
};
struct MAKINA_EXPORT sampler_description
{
  std::array<GLenum, 3>  wrap         = {GL_REPEAT, GL_REPEAT, GL_REPEAT};
  GLenum                 min_filter   = GL_NEAREST;
  GLenum                 mag_filter   = GL_NEAREST;
};
struct MAKINA_EXPORT render_target_description
{
  std::array<GLsizei, 2> size         ;
  GLenum                 color_format = GL_RGBA;
  GLenum                 depth_format = GL_DEPTH_COMPONENT32;
};

using buffer_resource        = fg::resource<buffer_description       , gl::buffer          >;
using texture_1d_resource    = fg::resource<texture_description      , gl::texture_1d      >;
using texture_2d_resource    = fg::resource<texture_description      , gl::texture_2d      >;
using texture_3d_resource    = fg::resource<texture_description      , gl::texture_3d      >;
using sampler_resource       = fg::resource<sampler_description      , gl::sampler         >;
using render_target_resource = fg::resource<render_target_description, opengl_render_target>;
}

template<>
inline std::unique_ptr<gl::buffer>                fg::realize(const mak::buffer_description&        description)
{
  auto   buffer = std::make_unique<gl::buffer>();
  buffer->set_data_immutable(description.size);
  buffer->bind              (description.type);
  buffer->unbind            (description.type);
  return buffer;
}
template<>
inline std::unique_ptr<gl::texture_1d>            fg::realize(const mak::texture_description&       description)
{
  auto   texture = std::make_unique<gl::texture_1d>();
  texture->set_storage(description.levels, description.format, description.size[0]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_2d>            fg::realize(const mak::texture_description&       description)
{
  auto   texture = std::make_unique<gl::texture_2d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_3d>            fg::realize(const mak::texture_description&       description)
{
  auto   texture = std::make_unique<gl::texture_3d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1], description.size[2]);
  return texture;
}
template<>
inline std::unique_ptr<gl::sampler>               fg::realize(const mak::sampler_description&       description)
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
inline std::unique_ptr<mak::opengl_render_target> fg::realize(const mak::render_target_description& description)
{
  return std::make_unique<mak::opengl_render_target>(description.size, description.color_format, description.depth_format);
}

#endif