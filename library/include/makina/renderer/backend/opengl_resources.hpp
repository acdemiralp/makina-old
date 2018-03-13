#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RESOURCES_HPP_

#include <array>

#include <fg/framegraph.hpp>
#include <gl/all.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT buffer_description
{
  std::size_t                size             ;
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
  std::array<std::size_t, 3> size             ;
  GLenum                     format           = GL_RGBA;
  std::size_t                levels           = 0      ;
};
struct MAKINA_EXPORT framebuffer_description
{
  std::array<std::size_t, 3> size             ;
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

#endif