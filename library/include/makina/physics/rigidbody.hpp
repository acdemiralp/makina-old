#ifndef MAKINA_PHYSICS_RIGIDBODY_HPP_
#define MAKINA_PHYSICS_RIGIDBODY_HPP_

#include <memory>

#include <btBulletDynamicsCommon.h>

#include <makina/export.hpp>

namespace mak
{
class physics_system;

class MAKINA_EXPORT rigidbody
{
public:
  float mass    () const;
  void  set_mass(const float value);

protected:
  friend physics_system;

  float                        mass_   = 1.0f;
  std::shared_ptr<btRigidBody> native_ ; // Managed internally by the physics system.
};
}

#endif