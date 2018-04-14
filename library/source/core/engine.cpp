#include <makina/core/engine.hpp>

namespace mak
{
engine::engine() : entry_node_(flow_graph_)
{

}

scene* engine::scene     () const
{
  return scene_.get();
}
void   engine::set_scene (std::unique_ptr<mak::scene> scene)
{
  scene_ = std::move(scene);
}

void   engine::run       ()
{
  if (is_running_)
  {
    logger->warn("No-op. Engine is already running.");
    return;
  }

  is_running_ = true;

  frame_timer_.tick        ();
  entry_node_.try_put     (flow_node_data{true, scene_.get()});
  for (auto& system : systems_) 
    if (!system->async()) 
      system->prepare(scene_.get());
  flow_graph_ .wait_for_all();

  while (is_running_)
  {
    frame_timer_.tick        ();
    entry_node_.try_put     (flow_node_data{false, scene_.get(), frame_timer_.delta_time()});
    for (auto& system : systems_) 
      if (!system->async()) 
        system->update(frame_timer_.delta_time(), scene_.get());
    flow_graph_ .wait_for_all();
  }
}
void   engine::stop      ()
{
  if (!is_running_)
  {
    logger->warn("No-op. Engine is already stopped.");
    return;
  }

  is_running_ = false;
}
bool   engine::is_running() const
{
  return is_running_;
}
}
