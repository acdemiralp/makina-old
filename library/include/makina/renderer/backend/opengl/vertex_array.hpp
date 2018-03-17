#ifndef MAKINA_RENDERER_BACKEND_OPENGL_VERTEX_ARRAY_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_VERTEX_ARRAY_HPP_

#include <gl/vertex_array.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT attribute_binding_description
{
  buffer_resource* buffer    ;
  GLint            components;
  GLenum           type      ;
  GLboolean        normalize = GL_FALSE;
  GLuint           divisor   = 0       ;
};
struct MAKINA_EXPORT vertex_array_description
{
  std::vector<attribute_binding_description> attribute_bindings     ;
  std::vector<buffer_resource*>              shader_storage_bindings;
  buffer_resource*                           element_buffer         = nullptr;
};
using vertex_array_resource = fg::resource<vertex_array_description, gl::vertex_array>;
}

template<>
inline std::unique_ptr<gl::vertex_array>              fg::realize(const mak::vertex_array_description&      description)
{
  auto vertex_array = std::make_unique<gl::vertex_array>();

  // Important: Vertex arrays must be realized after all of the referred buffers are realized.
  for(auto i = 0; i < description.attribute_bindings.size(); ++i)
  {
    auto& binding = description.attribute_bindings[i];
    vertex_array->set_vertex_buffer    (i, *binding.buffer->actual(), 0, binding.components * gl::type_size(binding.type));
    vertex_array->set_attribute_enabled(i, true);

    if(binding.type == GL_BYTE  || binding.type == GL_UNSIGNED_BYTE  ||
       binding.type == GL_SHORT || binding.type == GL_UNSIGNED_SHORT ||
       binding.type == GL_INT   || binding.type == GL_UNSIGNED_INT   || binding.type == GL_FIXED )
      vertex_array->set_attribute_format_integer(i, binding.components, binding.type, 0);
    else if(binding.type == GL_DOUBLE)
      vertex_array->set_attribute_format_long   (i, binding.components, binding.type, 0);
    else // if(binding.type == GL_FLOAT || binding.type == GL_HALF_FLOAT)
      vertex_array->set_attribute_format        (i, binding.components, binding.type, false, 0);

    vertex_array->set_binding_divisor(i, binding.divisor);
  }

  if (description.element_buffer)
    vertex_array->set_element_buffer(*description.element_buffer->actual());

  return vertex_array;
}

#endif