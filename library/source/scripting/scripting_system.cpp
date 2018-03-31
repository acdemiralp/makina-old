#include <makina/scripting/scripting_system.hpp>

#include <makina/scripting/behavior.hpp>

namespace mak
{
void scripting_system::prepare(scene* scene)
{
  for(auto& entity : scene->entities<behaviors>())
    for (auto& behavior : *entity->component<behaviors>())
    {
      if(behavior->on_prepare_)
        behavior->on_prepare_(scene, entity);
    }
}
void scripting_system::update (frame_timer::duration delta, scene* scene)
{
  for (auto& entity : scene->entities<behaviors>())
    for (auto& behavior : *entity->component<behaviors>())
      if(behavior->on_update_)
        behavior->on_update_(delta, scene, entity);
}
}
