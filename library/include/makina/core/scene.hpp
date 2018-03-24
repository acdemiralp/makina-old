#ifndef MAKINA_CORE_SCENE_HPP_
#define MAKINA_CORE_SCENE_HPP_

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/input/controller.hpp>
#include <makina/physics/collider.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
using entity = ec::entity<controller, collider, rigidbody, light, mesh_render, projection, transform>;
using scene  = ec::scene<entity>;
}

#endif