#include <makina/physics/rigidbody.hpp>

namespace mak
{
float     rigidbody::mass        () const
{
  return mass_   ;
}
void      rigidbody::set_mass    (const float      value)
{
  mass_    = value;
}

glm::vec3 rigidbody::velocity    () const
{
  return native_ ? glm::vec3(native_->getLinearVelocity()[0], native_->getLinearVelocity()[1], native_->getLinearVelocity()[2]) : glm::vec3();
}
void      rigidbody::set_velocity(const glm::vec3& value) const
{
  if(native_) native_->setLinearVelocity({value[0], value[1], value[2]});
}
}
