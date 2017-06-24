#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_RENDERBUFFER_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_RENDERBUFFER_HPP_

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
template<GLenum type>
class texture;

class MAKINA_EXPORT renderbuffer
{
public:
  // 9.2.4 Renderbuffer objects.
  renderbuffer();
  renderbuffer(GLuint id);
  renderbuffer(const renderbuffer&  that);
  renderbuffer(      renderbuffer&& temp) = default;
  virtual ~renderbuffer();
  renderbuffer& operator=(const renderbuffer&  that);
  renderbuffer& operator=(      renderbuffer&& temp) = default;

  void        bind    () const;
  static void unbind  ();
  bool        is_valid() const;

  void set_storage            (                 GLenum internal_format, GLsizei width, GLsizei height);
  void set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height);

  // 9.2.6 Renderbuffer object queries (bindless).
  GLsizei width          () const;
  GLsizei height         () const;
  GLenum  internal_format() const;
  GLsizei samples        () const;
  GLsizei red_size       () const;
  GLsizei green_size     () const;
  GLsizei blue_size      () const;
  GLsizei alpha_size     () const;
  GLsizei depth_size     () const;
  GLsizei stencil_size   () const;
  
  // 18.3 Copying pixels.
  template<GLenum source_target>
  void copy_image_sub_data(const texture<source_target>& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth);
  void copy_image_sub_data(const renderbuffer& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth);

  static const GLenum native_type = GL_RENDERBUFFER;

  GLuint id() const;

protected:
  GLint get_parameter(GLenum parameter) const;

  GLuint id_      ;
  bool   managed_ = true;
};

template <GLenum source_target>
void renderbuffer::copy_image_sub_data(
  const texture<source_target>& source,
  GLint source_level, GLint source_x, GLint source_y, GLint source_z,
  GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
{
  glCopyImageSubData(source.id(), source_target, source_level, source_x, source_y, source_z, id_, GL_RENDERBUFFER, level, x, y, z, width, height, depth);
}
}
}

#endif
