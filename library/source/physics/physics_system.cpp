#include <makina/physics/physics_system.hpp>

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

namespace mak
{
physics_system::physics_system()
: configuration_()
, dispatcher_   (&configuration_)
, broadphase_   ()
, solver_       ()
, world_        (&dispatcher_, &broadphase_, &solver_, &configuration_)
{
  btGImpactCollisionAlgorithm::registerAlgorithm(&dispatcher_);

  world_.setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

btDiscreteDynamicsWorld* physics_system::world()
{
  return &world_;
}

void physics_system::prepare()
{
  // TODO: Collect rigidbodies/colliders from the scene.
}
void physics_system::update (frame_timer::duration delta, scene* scene)
{
  world_.stepSimulation(delta.count(), 60, 1.0f / 60.0f);
}
}
