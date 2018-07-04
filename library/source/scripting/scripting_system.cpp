#include <makina/scripting/scripting_system.hpp>

#include <tbb/tbb.h>

#include <makina/scripting/behavior.hpp>

namespace mak
{
void scripting_system::prepare(                                   scene* scene)
{
  const auto entities = scene->entities<behaviors>();
  tbb::parallel_for(std::size_t(0), entities.size(), std::size_t(1), [=] (std::size_t i)
  {
    for (auto& behavior : *entities[i]->component<behaviors>())
      if (behavior->on_prepare_)
        behavior->on_prepare_(scene, entities[i]);
  });
}
void scripting_system::update (const frame_timer::duration delta, scene* scene)
{
  const auto entities = scene->entities<behaviors>();
  tbb::parallel_for(std::size_t(0), entities.size(), std::size_t(1), [=] (std::size_t i)
  {
    for (auto& behavior : *entities[i]->component<behaviors>())
      if (behavior->on_update_)
        behavior->on_update_(delta, scene, entities[i]);
  });
}
}
