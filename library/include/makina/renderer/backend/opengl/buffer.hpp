#ifndef MAKINA_RENDERER_BACKEND_OPENGL_BUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_BUFFER_HPP_

#include <fg/resource.hpp>
#include <gl/buffer.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT buffer_description
{
  GLsizeiptr size ;
  GLenum     type = GL_ARRAY_BUFFER;
};
using buffer_resource = fg::resource<buffer_description, gl::buffer>;
}

template<>
inline std::unique_ptr<gl::buffer> fg::realize(const mak::buffer_description& description)
{
  auto   buffer = std::make_unique<gl::buffer>();
  buffer->set_data_immutable(description.size);
  buffer->bind              (description.type);
  buffer->unbind            (description.type);
  return buffer;
}

#endif