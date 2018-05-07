#ifndef MAKINA_RENDERER_BACKEND_OPENGL_VERTEX_ARRAY_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_VERTEX_ARRAY_HPP_

#include <fg/resource.hpp>
#include <gl/vertex_array.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
// A gl::vertex_array with support for recording shader storage and draw indirect buffers.
class MAKINA_EXPORT vertex_array : public gl::vertex_array
{
public:
  struct MAKINA_EXPORT attribute_binding
  {
    buffer_resource* buffer          ;
    GLint            components      ;
    GLenum           type            ;
    GLboolean        normalize       = GL_FALSE;
    GLuint           offset          = 0;
    GLsizei          stride          = 0;
    GLuint           relative_offset = 0;
    GLuint           divisor         = 0;
  };
  struct MAKINA_EXPORT description
  {
    std::vector<attribute_binding> attribute_bindings     ;
    std::vector<buffer_resource*>  shader_storage_buffers ;
    buffer_resource*               element_buffer         = nullptr;
    buffer_resource*               draw_indirect_buffer   = nullptr;
  };

  explicit vertex_array  (const description& description);
  vertex_array           (const vertex_array&  that) = default;
  vertex_array           (      vertex_array&& temp) = default;
  virtual ~vertex_array  ()                          = default;
  vertex_array& operator=(const vertex_array&  that) = default;
  vertex_array& operator=(      vertex_array&& temp) = default;

  void bind  () const;
  void unbind() const;

protected:
  std::vector<buffer_resource*> shader_storage_buffers_;
  buffer_resource*              draw_indirect_buffer_  ;
};
using vertex_array_resource = fg::resource<vertex_array::description, vertex_array>;
}
}

template<>
inline std::unique_ptr<mak::opengl::vertex_array> fg::realize(const mak::opengl::vertex_array::description& description)
{
  return std::make_unique<mak::opengl::vertex_array>(description);
}

#endif