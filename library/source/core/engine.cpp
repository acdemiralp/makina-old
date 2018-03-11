#include <makina/core/engine.hpp>

namespace mak
{
void engine::set_scene (std::unique_ptr<scene> scene)
{
  scene_ = std::move(scene);
}
void engine::run       ()
{
  if (is_running_)
  {
    logger->warn("No-op. Engine is already running.");
    return;
  }

  is_running_ = true;

  frame_timer_.tick();
  for (auto& system : systems_)
    system->prepare();

  while (is_running_)
  {
    frame_timer_.tick();
    for (auto& system : systems_)
      system->update(frame_timer_.delta_time(), scene_.get());
  }
}
void engine::stop      ()
{
  if (!is_running_)
  {
    logger->warn("No-op. Engine is already stopped.");
    return;
  }

  is_running_ = false;
}
bool engine::is_running() const
{
  return is_running_;
}
}
