#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_TESSELLATION_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_TESSELLATION_HPP_

#include <vector>

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 10.1.15 Separate patches.
MAKINA_EXPORT void set_patch_vertex_count(GLint count);

// 11.2.2 Tessellation primitive generation.
MAKINA_EXPORT void set_patch_default_inner_level(const std::vector<float>& inner_level);
MAKINA_EXPORT void set_patch_default_outer_level(const std::vector<float>& outer_level);
}
}

#endif
