#include <makina/core/rendering/backends/opengl/viewport.hpp>

namespace mak
{
namespace gl
{
// 13.6.1 Controlling viewport.
void set_depth_range        (              double near, double far)
{
  glDepthRange(near, far);
}
void set_depth_range        (              float  near, float  far)
{
  glDepthRangef(near, far);
}
void set_indexed_depth_range(GLuint index, double near, double far)
{
  glDepthRangeIndexed(index, near, far);
}
void set_viewport           (              const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glViewport(offset[0], offset[1], size[0], size[1]);
}
void set_indexed_viewport   (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glViewportIndexedf(index, offset[0], offset[1], size[0], size[1]);
}
}
}