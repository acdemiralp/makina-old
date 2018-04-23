#include <makina/core/scene.hpp>

#include <makina/core/logger.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
void append_scene(scene* source, scene* target)
{
  auto source_entities = source->entities();
  auto target_entities = std::vector<entity*>();
  
  const std::function<void(entity*, entity*)> recursive_add_entity = [&] (mak::entity* source_entity, mak::entity* parent)
  {
    auto entity    = target->copy_entity(source_entity);
    auto transform = entity->component<mak::transform>();
    auto metadata  = entity->component<mak::metadata> ();
    transform->set_metadata(metadata);
    transform->set_children({});
    if (parent) transform->set_parent(parent->component<mak::transform>());

    target_entities.push_back(entity);

    for (auto child : source_entity->component<mak::transform>()->children())
      recursive_add_entity(*std::find_if(
        source_entities.begin(),
        source_entities.end  (),
        [&] (mak::entity* iteratee)
        {
          return child == iteratee->component<mak::transform>();
        }), 
        entity);
  };

  for (auto& entity : source_entities)
    if (!entity->component<transform>()->parent())
      recursive_add_entity(entity, nullptr);

  bone* root_bone = nullptr;
  for (auto& entity : target_entities)
    if (entity->component<metadata>()->contains_tag("root_bone"))
      root_bone = entity->component<bone>();
  for (auto& entity : target_entities)
    if (entity->has_components<mak::animator>())
      entity->component<animator>()->root_bone = root_bone;
}
void print_scene (const scene* scene)
{
  const std::function<void(entity*, std::size_t)> recursive_print = [&] (entity* entity, std::size_t depth)
  {
    if (entity->has_components<metadata>())
      logger->info("{}- {} ({})", depth > 0 ? std::string(depth, ' ') : "", entity->component<metadata>()->name, entity->bitset().to_string());

    auto entities = scene ->entities <transform>();
    auto children = entity->component<transform>()->children();
    for (auto child : children)
      recursive_print(*std::find_if(entities.begin(), entities.end(), [&] (mak::entity* iteratee) {return iteratee->component<transform>() == child;}), depth + 1);
  };

  logger->info("{}", std::string(50, '#'));

  logger->info("Transformless Entities");
  for (auto entity : scene->entities())
    if (!entity->has_components<transform>() && entity->has_components<metadata>())
      logger->info("- {} ({})", entity->component<metadata>()->name, entity->bitset().to_string());

  logger->info("{}", std::string(50, '#'));

  logger->info("Transform Hierarchy");
  for (auto entity : scene->entities())
    if ( entity->has_components<transform>() && !entity->component<transform>()->parent())
      recursive_print(entity, 0);

  logger->info("{}", std::string(50, '#'));
}
}