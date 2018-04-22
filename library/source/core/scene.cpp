#include <makina/core/scene.hpp>

#include <makina/core/logger.hpp>

namespace mak
{
void append_scene(scene* source, scene* target)
{
  auto entities = target->append(*source);
  for (auto& entity : entities)
  {
    auto transform = entity->component<mak::transform>();
    if (!transform->parent()) continue;

    auto parent_entity = *std::find_if(entities.begin(), entities.end(), [&] (mak::entity* iteratee)
    {
      return transform->parent()->metadata()->name == iteratee->component<mak::metadata>()->name;
    });

    auto parent_metadata  = parent_entity->component<mak::metadata> ();
    auto parent_transform = parent_entity->component<mak::transform>();
    parent_transform->set_metadata(parent_metadata );
    transform       ->set_parent  (parent_transform);
  }
}
void print_scene (const scene* scene)
{
  const std::function<void(entity*, std::size_t)> recursive_print = [&] (entity* entity, std::size_t depth)
  {
    if (entity->has_components<metadata>())
      logger->info("{}- {}", depth > 0 ? std::string(depth, ' ') : "", entity->component<metadata>()->name);

    auto entities = scene ->entities <transform>();
    auto children = entity->component<transform>()->children();
    for (auto child : children)
      recursive_print(*std::find_if(entities.begin(), entities.end(), [&] (mak::entity* iteratee) {return iteratee->component<transform>() == child;}), depth + 1);
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