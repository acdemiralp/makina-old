#include <makina/core/rendering/backends/opengl/compute.hpp>

namespace mak
{
namespace gl
{
void dispatch_compute         (GLuint grid_x, GLuint grid_y, GLuint grid_z)
{
  glDispatchCompute(grid_x, grid_y, grid_z);
}
void dispatch_compute_indirect(GLintptr offset)
{
  glDispatchComputeIndirect(offset);
}
}
}