#ifndef MAKINA_CORE_SCENE_FWD_HPP_
#define MAKINA_CORE_SCENE_FWD_HPP_

#include <memory>
#include <vector>

namespace ec
{
template<typename... types>
class entity;

template<typename type>
class scene;
}

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
struct line_render   ;
struct mesh_render   ;
struct point_render  ;
class  projection    ;
class  transform     ;
struct volume_render ;

using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity<metadata, audio_listener, audio_source, controller, mesh_collider, rigidbody, animator, bone, light, line_render, mesh_render, point_render, projection, transform, volume_render, behaviors>;
using scene     = ec::scene<entity>;
}

#endif