#ifndef MAKINA_PHYSICS_PHYSICS_SYSTEM_HPP_
#define MAKINA_PHYSICS_PHYSICS_SYSTEM_HPP_

#include <memory>
#include <vector>
#include <unordered_set>

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

  btDiscreteDynamicsWorld* world() const;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;

  std::unique_ptr<btDefaultCollisionConfiguration>      configuration_				;
  std::unique_ptr<btCollisionDispatcher>                dispatcher_						;
  std::unique_ptr<btDbvtBroadphase>                     broadphase_						;
  std::unique_ptr<btSequentialImpulseConstraintSolver>  solver_								;
  std::unique_ptr<btDiscreteDynamicsWorld>              world_								;

  std::vector<rigidbody*>                               rigidbodies_					;
  std::vector<std::pair<transform*, mesh_collider*>>    mesh_colliders_				;

	std::unordered_set<std::size_t>												prepared_entity_ids_	;

private:
	void																									prepare_new_entities(scene* scene)		;
	void																									prepare_entity(entity* entity)				;
};
}

#endif