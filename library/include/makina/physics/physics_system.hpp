#ifndef MAKINA_PHYSICS_PHYSICS_SYSTEM_HPP_
#define MAKINA_PHYSICS_PHYSICS_SYSTEM_HPP_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT physics_system : public system
{
public:
  physics_system           ();
  physics_system           (const physics_system&  that) = delete ;
  physics_system           (      physics_system&& temp) = default;
  virtual ~physics_system  ()                            = default;
  physics_system& operator=(const physics_system&  that) = delete ;
  physics_system& operator=(      physics_system&& temp) = default;

  btDiscreteDynamicsWorld* world();

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;

  btDefaultCollisionConfiguration     configuration_;
  btCollisionDispatcher               dispatcher_   ;
  btDbvtBroadphase                    broadphase_   ;
  btSequentialImpulseConstraintSolver solver_       ;
  btDiscreteDynamicsWorld             world_        ;
};
}

#endif