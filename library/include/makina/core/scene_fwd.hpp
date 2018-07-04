#ifndef MAKINA_CORE_SCENE_FWD_HPP
#define MAKINA_CORE_SCENE_FWD_HPP

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
class  audio_listener ;
class  audio_source   ;
class  behavior       ;
struct metadata       ;
struct controller     ;
class  mesh_collider  ;
class  rigidbody      ;
struct animator       ;
struct bone           ;
struct light          ;
struct line_render    ;
struct mesh_render    ;
struct point_render   ;
class  projection     ;
class  transform      ;
struct volume_render  ;

using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity< 
  // Common
  metadata      , 
  transform     ,
  // Rendering 
  point_render  , 
  line_render   , 
  mesh_render   , 
  volume_render ,
  light         ,  
  projection    ,
  animator      , 
  bone          ,
  // Audio
  audio_listener, 
  audio_source  , 
  // Physics
  rigidbody     ,
  mesh_collider , 
  // Scripting
  behaviors     ,
  controller    >;
using scene     = ec::scene<entity>;
}

#endif