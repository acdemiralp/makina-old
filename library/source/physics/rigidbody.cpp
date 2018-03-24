#include <makina/physics/rigidbody.hpp>

namespace mak
{
float rigidbody::mass    () const
{
  return mass_   ;
}
void  rigidbody::set_mass(const float value)
{
  mass_    = value;
}
}
