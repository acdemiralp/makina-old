#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_SYNC_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_SYNC_HPP_

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
class MAKINA_EXPORT sync
{
public:
  // 4.1 Sync objects and fences.
  sync();
  sync(const sync&  that) = delete ;
  sync(      sync&& temp) = default;
  virtual ~sync();
  sync& operator=(const sync&  that) = delete ;
  sync& operator=(      sync&& temp) = default;

  // 4.1.1 Waiting for sync objects.
  GLenum client_wait(GLbitfield flags = GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64 timeout_ns = 10E+10) const;
  void   wait       () const;

  // 4.1.3 Sync object queries.
  GLenum status  () const;
  bool   is_valid() const;

  GLsync id() const;

protected:
  GLint get_property(GLenum property) const;

  GLsync id_;
};
}
}

#endif