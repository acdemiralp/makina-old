#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_COMMAND_EXECUTION_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_COMMAND_EXECUTION_HPP_

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

// 2.3 Command execution.
namespace mak
{
namespace gl
{
// 2.3.1 OpenGL errors.
MAKINA_EXPORT GLenum error();

// 2.3.2 Graphics reset recovery.
MAKINA_EXPORT GLenum graphics_reset_status();
MAKINA_EXPORT GLenum reset_notification_strategy();

// 2.3.3 Flush and finish.
MAKINA_EXPORT void flush ();
MAKINA_EXPORT void finish();
}
}

#endif