#include <makina/core/rendering/backends/opengl/vertex_post_processing_ops.hpp>

namespace mak
{
namespace gl
{
// 13.4 Flatshading.
void set_provoking_vertex(GLenum provoke_mode)
{
  glProvokingVertex(provoke_mode);
}

// 13.5 Primitive clipping.
void set_depth_clamp_enabled  (bool enabled)
{
  enabled ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
}
bool depth_clamp_enabled      ()
{
  return glIsEnabled(GL_DEPTH_CLAMP);
}
void set_clip_distance_enabled(bool enabled , GLuint index)
{
  enabled ? glEnable(GL_CLIP_DISTANCE0 + index) : glDisable(GL_CLIP_DISTANCE0 + index);
}
bool clip_distance_enabled    (               GLuint index)
{
  return glIsEnabled(GL_CLIP_DISTANCE0 + index);
}
void set_clip_control         (GLenum origin, GLenum depth)
{
  glClipControl(origin, depth);
}
}
}