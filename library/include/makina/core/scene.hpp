#ifndef MAKINA_CORE_SCENE_HPP
#define MAKINA_CORE_SCENE_HPP

#include <memory>
#include <vector>

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/audio/audio_listener.hpp>
#include <makina/audio/audio_source.hpp>
#include <makina/core/metadata.hpp>
#include <makina/input/controller.hpp>
#include <makina/physics/mesh_collider.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/animator.hpp>
#include <makina/renderer/bone.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/line_render.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/point_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/renderer/volume_render.hpp>

namespace mak
{
class behavior  ;
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

MAKINA_EXPORT void append_scene(scene* source, scene* target);
MAKINA_EXPORT void print_scene (const scene* scene);
}

#endif