#ifndef MAKINA_CORE_SCENE_HPP_
#define MAKINA_CORE_SCENE_HPP_

#include <memory>
#include <string>
#include <vector>

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/audio/audio_listener.hpp>
#include <makina/audio/audio_source.hpp>
#include <makina/core/logger.hpp>
#include <makina/core/metadata.hpp>
#include <makina/input/controller.hpp>
#include <makina/physics/mesh_collider.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/animator.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
class behavior  ;
using behaviors = std::vector<std::shared_ptr<behavior>>;

using entity    = ec::entity<metadata, audio_listener, audio_source, controller, mesh_collider, rigidbody, animator, light, mesh_render, projection, transform, behaviors>;
using scene     = ec::scene<entity>;

inline void print_scene(const scene* scene)
{
  const std::function<void(entity*, std::size_t)> recursive_print = [&] (entity* entity, std::size_t depth)
  {
    if (entity->has_components<metadata>())
      logger->info("{}- {}", depth > 0 ? std::string(depth, ' ') : "", entity->component<metadata>()->name);
    for (auto& child : entity->component<transform>()->children())
    {
      auto entities = scene->entities<transform>();
      recursive_print(*std::find_if(entities.begin(), entities.end(), [&] (mak::entity* iteratee) {return iteratee->component<transform>() == child;}), depth + 1);
    }
  };

  logger->info("{}", std::string(50, '#'));
  logger->info("Transformless Entities");
  for (auto entity : scene->entities())
    if (!entity->has_components<transform>() && entity->has_components<metadata>())
      logger->info("- {}", entity->component<metadata>()->name);

  logger->info("{}", std::string(50, '#'));
  logger->info("Transform Hierarchy");
  for (auto entity : scene->entities())
    if ( entity->has_components<transform>() && !entity->component<transform>()->parent())
      recursive_print(entity, 0);

  logger->info("{}", std::string(50, '#'));
}
}

#endif