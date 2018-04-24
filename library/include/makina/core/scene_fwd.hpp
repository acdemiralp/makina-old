#ifndef MAKINA_CORE_SCENE_FWD_HPP_
#define MAKINA_CORE_SCENE_FWD_HPP_

#include <memory>
#include <vector>

#include <ec/entity.hpp>
#include <ec/scene.hpp>

namespace mak
{
class  audio_listener;
class  audio_source  ;
class  behavior      ;
struct metadata      ;
struct controller    ;
class  mesh_collider ;
class  rigidbody     ;
struct animator      ;
struct bone          ;
struct light         ;
struct mesh_render   ;
class  projection    ;
class  transform     ;

using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity<metadata, audio_listener, audio_source, controller, mesh_collider, rigidbody, animator, bone, light, mesh_render, projection, transform, behaviors>;
using scene     = ec::scene<entity>;
}

#endif