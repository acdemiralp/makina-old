#include <makina/scripting/scripting_system.hpp>

namespace mak
{
void scripting_system::prepare(scene* scene)
{
  for(auto& entity : scene->entities<behaviors>())
    for (auto& behavior : *entity->component<behaviors>())
      behavior->on_prepare();
}
void scripting_system::update (frame_timer::duration delta, scene* scene)
{
  for (auto& entity : scene->entities<behaviors>())
    for (auto& behavior : *entity->component<behaviors>())
      behavior->on_update(delta);
}
}
