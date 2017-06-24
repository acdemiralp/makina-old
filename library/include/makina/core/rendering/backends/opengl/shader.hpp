#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_SHADER_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_SHADER_HPP_

#include <fstream>
#include <string>

#include <makina/core/rendering/backends/opengl/opengl.hpp>
#include <makina/core/rendering/backends/opengl/unmanaged.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
class MAKINA_EXPORT shader
{
public:
  // 7.1 Shader objects.
  shader(GLenum type);
  shader(GLuint id, unmanaged_t unmanaged);
  shader(const shader&  that) = delete ;
  shader(      shader&& temp) = default;
  virtual ~shader();
  shader& operator=(const shader&  that) = delete ;
  shader& operator=(      shader&& temp) = default;

  void set_source(const std::string& source) const;
  bool compile   () const;
  bool is_valid  () const;

  static void release_compiler();

  // 7.13 Shader queries.
  GLenum      type           () const;
  bool        compile_status () const;
  bool        delete_status  () const;
  GLsizei     source_length  () const;
  GLsizei     info_log_length() const;
  std::string source         () const;
  std::string info_log       () const;

  static const GLenum native_type = GL_SHADER;

  GLuint id() const;

  // Extended functionality.
  void load_source(const std::string& filename) const;

protected:
  GLint get_parameter(GLenum parameter) const;

  GLuint id_      = 0;
  bool   managed_ = true;
};
}
}
#endif
