#include <makina/physics/mesh_collider.hpp>

namespace mak
{
mesh*     mesh_collider::mesh    () const
{
  return mesh_;
}
void      mesh_collider::set_mesh(mak::mesh* mesh)
{
  mesh_ = mesh;
}

glm::mat4 mesh_collider::matrix  () const
{
  btTransform transform;
  glm::mat4   matrix   ;
  motion_state_->getWorldTransform(transform    );
  transform    . getOpenGLMatrix  (&matrix[0][0]);
  return matrix;
}
}
