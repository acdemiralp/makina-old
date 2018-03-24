#include <makina/physics/physics_system.hpp>

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include <makina/resources/mesh.hpp>

namespace mak
{
physics_system::physics_system()
: configuration_(std::make_unique<btDefaultCollisionConfiguration>    ())
, dispatcher_   (std::make_unique<btCollisionDispatcher>              (configuration_.get()))
, broadphase_   (std::make_unique<btDbvtBroadphase>                   ())
, solver_       (std::make_unique<btSequentialImpulseConstraintSolver>())
, world_        (std::make_unique<btDiscreteDynamicsWorld>            (dispatcher_.get(), broadphase_.get(), solver_.get(), configuration_.get()))
{
  btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher_.get());

  world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

btDiscreteDynamicsWorld* physics_system::world() const
{
  return world_.get();
}

void physics_system::prepare(scene* scene)
{
  for (const auto rigidbody : rigidbodies_)
    world_->removeRigidBody(rigidbody->native_.get());
  rigidbodies_.clear();

  auto entities = scene->entities<transform, mesh_collider, rigidbody>();
  for(auto entity : entities)
  {
    auto transform     = entity->component<mak::transform>    ();
    auto mesh_collider = entity->component<mak::mesh_collider>();
    auto rigidbody     = entity->component<mak::rigidbody>    ();

    btTransform initial_transform;
    initial_transform.setFromOpenGLMatrix(&transform->matrix(true)[0][0]);

    mesh_collider->native_       = std::make_shared<btConvexHullShape>   (&mesh_collider->mesh()->vertices.data()->x, mesh_collider->mesh()->vertices.size(), sizeof glm::vec3);
    mesh_collider->motion_state_ = std::make_shared<btDefaultMotionState>(initial_transform);

    btVector3 inertia;
    mesh_collider->native_->calculateLocalInertia(rigidbody->mass(), inertia);

    rigidbody->native_ = std::make_shared<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo(
      rigidbody    ->mass(), 
      mesh_collider->motion_state_.get(),
      mesh_collider->native_      .get(),
      inertia));
    world_->addRigidBody(rigidbody->native_.get());

    rigidbodies_   .push_back(rigidbody);
    mesh_colliders_.push_back({transform, mesh_collider});
  }
}
void physics_system::update (frame_timer::duration delta, scene* scene)
{
  world_->stepSimulation(delta.count());
  for (auto& object : mesh_colliders_)
    object.first->set_matrix(object.second->matrix(), true);
}
}
