#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_VERTEX_POST_PROCESSING_OPS_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_VERTEX_POST_PROCESSING_OPS_HPP_

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 13.4 Flatshading.
MAKINA_EXPORT void set_provoking_vertex(GLenum provoke_mode);

// 13.5 Primitive clipping.
MAKINA_EXPORT void set_depth_clamp_enabled  (bool enabled);
MAKINA_EXPORT bool depth_clamp_enabled      ();
MAKINA_EXPORT void set_clip_distance_enabled(bool enabled , GLuint index = 0);
MAKINA_EXPORT bool clip_distance_enabled    (               GLuint index = 0);
MAKINA_EXPORT void set_clip_control         (GLenum origin, GLenum depth);
}
}

#endif