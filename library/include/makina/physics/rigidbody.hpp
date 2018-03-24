#ifndef MAKINA_PHYSICS_RIGIDBODY_HPP_
#define MAKINA_PHYSICS_RIGIDBODY_HPP_

#include <memory>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <makina/export.hpp>

namespace mak
{
class physics_system;

class MAKINA_EXPORT rigidbody
{
public:
  float     mass       () const;
  glm::vec3 inertia    () const;

  void      set_mass   (const float      value);
  void      set_inertia(const glm::vec3& value);

  glm::mat4 matrix     () const;

protected:
  friend physics_system;

  float                                 mass_        = 1.0f;
  glm::vec3                             inertia_     ;

  // The following are managed internally by the physics system.
  std::shared_ptr<btRigidBody>          native_      ;
  std::shared_ptr<btConvexHullShape>    shape_       ;
  std::shared_ptr<btDefaultMotionState> motion_state_;
};
}

#endif