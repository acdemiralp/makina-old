#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_TRANSFORM_FEEDBACK_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_TRANSFORM_FEEDBACK_HPP_

#include <makina/core/rendering/backends/opengl/buffer.hpp>
#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
class MAKINA_EXPORT transform_feedback
{
public:
  // 13.2 Transform feedback.
  transform_feedback();
  transform_feedback(GLuint id);
  transform_feedback(const transform_feedback&  that) = delete ;
  transform_feedback(      transform_feedback&& temp) = default;
  virtual ~transform_feedback();
  transform_feedback& operator=(const transform_feedback&  that) = delete ;
  transform_feedback& operator=(      transform_feedback&& temp) = default;
  
  void        bind    () const;
  static void unbind  ();
  bool        is_valid() const;

  void set_buffer_range(GLuint index, const buffer& buffer, GLintptr offset, GLsizeiptr size);
  void set_buffer_base (GLuint index, const buffer& buffer);
  
  static void begin (GLenum primitive_mode = GL_TRIANGLES);
  static void pause ();
  static void resume();
  static void end   ();

  // 13.2.3 Transform feedback drawing.
  void draw                 (GLenum mode)                                        const;
  void draw_instanced       (GLenum mode,                GLsizei instance_count) const;
  void draw_stream          (GLenum mode, GLuint stream)                         const;
  void draw_stream_instanced(GLenum mode, GLuint stream, GLsizei instance_count) const;

  static const GLenum native_type = GL_TRANSFORM_FEEDBACK;

  GLuint id() const;

private:
  GLuint id_      = 0;
  bool   managed_ = true;
};
}
}

#endif
