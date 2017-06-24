#include <makina/core/rendering/backends/opengl/command_execution.hpp>

namespace mak
{
namespace gl
{
// 2.3.1 OpenGL errors.
GLenum error()
{
  return glGetError();
}

// 2.3.2 Graphics reset recovery.
GLenum graphics_reset_status()
{
  return glGetGraphicsResetStatus();
}
GLenum reset_notification_strategy()
{
  GLint value;
  glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY, &value);
  return value;
}

// 2.3.3 Flush and finish.
void flush ()
{
  glFlush();
}
void finish()
{
  glFinish();
}   
}
}