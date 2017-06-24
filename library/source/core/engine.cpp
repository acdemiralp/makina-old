#include <makina/core/engine.hpp>

namespace mak
{
std::vector<system*> engine::systems() const
{
  std::vector<system*> raw_systems(systems_.size());
  std::transform(systems_.begin(), systems_.end(), raw_systems.begin(),
  [](const std::unique_ptr<system>& iteratee)
  {
    return iteratee.get();
  });
  return raw_systems;
}
scene*               engine::scene  () const
{
  return scene_.get();
}

void engine::set_scene (std::unique_ptr<mak::scene> scene)
{
  scene_ = std::move(scene);
}

void engine::run       ()
{
  if(is_running_)
    return;

  initialize();

  is_running_ = true;

  frame_timer_.tick();
  while(is_running_)
  {
    frame_timer_.tick();
    update(frame_timer_.delta_time());
  }
  
  terminate();
}
void engine::stop      ()
{
  is_running_ = false;
}
bool engine::is_running() const
{
  return is_running_;
}

void engine::initialize()
{
  for(auto& system : systems_)
    system->initialize();
}
void engine::update    (float time)
{
  for (auto& system : systems_)
    system->update(time);
}
void engine::terminate ()
{
  for (auto& system : systems_)
    system->terminate();
}
}
