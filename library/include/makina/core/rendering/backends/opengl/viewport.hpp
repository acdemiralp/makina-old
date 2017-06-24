#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_VIEWPORT_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_VIEWPORT_HPP_

#include <array>

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 13.6.1 Controlling viewport.
MAKINA_EXPORT void set_depth_range        (              double near, double far);
MAKINA_EXPORT void set_depth_range        (              float  near, float  far);
MAKINA_EXPORT void set_indexed_depth_range(GLuint index, double near, double far);
MAKINA_EXPORT void set_viewport           (              const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);
MAKINA_EXPORT void set_indexed_viewport   (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);
}
}

#endif