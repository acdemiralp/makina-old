#ifndef MAKINA_RENDERER_BACKEND_OPENGL_TEXTURE_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_TEXTURE_HPP_

#include <fg/resource.hpp>
#include <gl/texture.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT texture_description
{
  std::array<GLsizei, 3> size   ;
  GLenum                 format = GL_RGBA;
  GLsizei                levels = 0      ;
};
using texture_1d_resource = fg::resource<texture_description, gl::texture_1d>;
using texture_2d_resource = fg::resource<texture_description, gl::texture_2d>;
using texture_3d_resource = fg::resource<texture_description, gl::texture_3d>;
}

template<>
inline std::unique_ptr<gl::texture_1d> fg::realize(const mak::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_1d>();
  texture->set_storage(description.levels, description.format, description.size[0]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_2d> fg::realize(const mak::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_2d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1]);
  return texture;
}
template<>
inline std::unique_ptr<gl::texture_3d> fg::realize(const mak::texture_description& description)
{
  auto   texture = std::make_unique<gl::texture_3d>();
  texture->set_storage(description.levels, description.format, description.size[0], description.size[1], description.size[2]);
  return texture;
}


#endif