#include <makina/physics/rigidbody.hpp>

namespace mak
{
float     rigidbody::mass       () const
{
  return mass_   ;
}
glm::vec3 rigidbody::inertia    () const
{
  return inertia_;
}

void      rigidbody::set_mass   (const float      value)
{
  mass_    = value;
}
void      rigidbody::set_inertia(const glm::vec3& value)
{
  inertia_ = value;
}

glm::mat4 rigidbody::matrix     () const
{
  btTransform transform;
  glm::mat4   matrix   ;
  motion_state_->getWorldTransform(transform);
  transform    . getOpenGLMatrix  (&matrix[0][0]);
  return matrix;
}
}
