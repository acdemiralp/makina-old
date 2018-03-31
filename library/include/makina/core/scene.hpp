#ifndef MAKINA_CORE_SCENE_HPP_
#define MAKINA_CORE_SCENE_HPP_

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/input/controller.hpp>
#include <makina/physics/mesh_collider.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/scripting/behavior.hpp>

namespace mak
{
using entity = ec::entity<controller, mesh_collider, rigidbody, light, mesh_render, projection, transform, behaviors>;
using scene  = ec::scene<entity>;
}

#endif