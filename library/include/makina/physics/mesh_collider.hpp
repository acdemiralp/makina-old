#ifndef MAKINA_PHYSICS_MESH_COLLIDER_HPP
#define MAKINA_PHYSICS_MESH_COLLIDER_HPP

#include <memory>

#include <btBulletCollisionCommon.h>
#include <glm/mat4x4.hpp>

#include <makina/export.hpp>

namespace mak
{
struct mesh;
class  physics_system;

class MAKINA_EXPORT mesh_collider
{
public:
  mesh*     mesh    () const;
  void      set_mesh(mak::mesh* mesh);

  glm::mat4 matrix  () const;

protected:
  friend physics_system;

  mak::mesh*                            mesh_        = nullptr;
  std::shared_ptr<btConvexHullShape>    native_      ; // Managed internally by the physics system.
  std::shared_ptr<btDefaultMotionState> motion_state_; // Managed internally by the physics system.
};
}

#endif