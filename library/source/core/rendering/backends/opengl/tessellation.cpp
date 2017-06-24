#include <makina/core/rendering/backends/opengl/tessellation.hpp>

namespace mak
{
namespace gl
{
// 10.1.15 Separate patches.
void set_patch_vertex_count(GLint count)
{
  glPatchParameteri(GL_PATCH_VERTICES, count);
}

// 11.2.2 Tessellation primitive generation.
void set_patch_default_inner_level(const std::vector<float>& inner_level)
{
  glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, inner_level.data());
}
void set_patch_default_outer_level(const std::vector<float>& outer_level)
{
  glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outer_level.data());
}
}
}