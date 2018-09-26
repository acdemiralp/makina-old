#include <makina/scripting/scripting_system.hpp>

#include <tbb/tbb.h>

#include <makina/scripting/behavior.hpp>

namespace mak
{
void scripting_system::prepare(                                   scene* scene)
{
  const auto entities = scene->entities<behaviors>();
  for (auto entity : entities)
    for (auto& behavior : *entity->component<behaviors>())
      if (behavior->on_prepare_)
        behavior->on_prepare_(scene, entity);
}
void scripting_system::update (const frame_timer::duration delta, scene* scene)
{
  const auto entities = scene->entities<behaviors>();
  for (auto entity : entities)
    for (auto& behavior : *entity->component<behaviors>())
      if (behavior->on_update_)
        behavior->on_update_(delta, scene, entity);
}
}
