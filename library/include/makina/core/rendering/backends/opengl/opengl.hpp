#ifndef GL_OPENGL_HPP_
#define GL_OPENGL_HPP_

#ifdef _WIN32
#  include <gl/glew.h>
#elif __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <gl/gl.h>
#endif

#include <export.hpp>

namespace mak
{
namespace gl
{
MAKINA_EXPORT void initialize ();
MAKINA_EXPORT void print_error(const char* prefix);

MAKINA_EXPORT GLsizei format_component_count(GLenum format);
MAKINA_EXPORT GLsizei type_size             (GLenum type  );
}
}

#endif