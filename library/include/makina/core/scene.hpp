#ifndef MAKINA_CORE_SCENE_HPP_
#define MAKINA_CORE_SCENE_HPP_

#include <memory>
#include <vector>

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/audio/audio_listener.hpp>
#include <makina/audio/audio_source.hpp>
#include <makina/input/controller.hpp>
#include <makina/physics/mesh_collider.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
class behavior  ;
using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity<audio_listener, audio_source, controller, mesh_collider, rigidbody, light, mesh_render, projection, transform, behaviors>;
using scene     = ec::scene<entity>;
}

#endif