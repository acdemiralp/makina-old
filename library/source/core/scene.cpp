#include <makina/core/scene.hpp>

#include <makina/core/logger.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
void append_scene(scene* source, scene* target)
{
  auto entities = target->append(*source);

  for (auto& entity : entities)
    entity->component<mak::transform>()->set_children({});

  for (auto i = 0; i < entities.size(); ++i)
  {
    const auto target_transform = entities[i]->component<mak::transform>();
    const auto target_metadata  = entities[i]->component<mak::metadata> ();
    target_transform->set_metadata(target_metadata);

    if (!target_transform->parent())
      continue;

    auto parent_entity = *std::find_if(entities.begin(), entities.end(), [&] (mak::entity* iteratee)
    {
      return target_transform->parent()->metadata()->name == iteratee->component<mak::metadata>()->name;
    });

    target_transform->set_parent(parent_entity->component<mak::transform>());
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