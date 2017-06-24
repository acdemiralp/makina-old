#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_COMPUTE_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_COMPUTE_HPP_

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 19.0 Compute shaders.
MAKINA_EXPORT void dispatch_compute         (GLuint grid_x, GLuint grid_y, GLuint grid_z);
MAKINA_EXPORT void dispatch_compute_indirect(GLintptr offset);
}
}

#endif