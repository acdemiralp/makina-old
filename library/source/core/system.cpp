#include <makina/core/system.hpp>

#include <stdexcept>

#include <makina/core/engine.hpp>

namespace mak
{
void system::set_async        (const bool async)
{
  if(async)
  {
    flow_node_.emplace(owner_->flow_graph_, 64, [&] (const flow_node_data& data)
    {
      data.prepare ? prepare(data.scene) : update(data.duration, data.scene);
      return data;
    });
    make_edge(owner_->entry_node_, *flow_node_);
  }
  else
    flow_node_ = std::nullopt;
}
bool system::async            () const
{
  return flow_node_.has_value();
}

void system::add_dependency   (system* dependency)
{
  if (!flow_node_.has_value())
    throw std::runtime_error("System must be set asynchronous before adding dependencies!"  );

  remove_edge(owner_->entry_node_   , *flow_node_);
  make_edge  (*dependency->flow_node_, *flow_node_);
}
void system::remove_dependency(system* dependency)
{
  if (!flow_node_.has_value())
    throw std::runtime_error("System must be set asynchronous before removing dependencies!");

  remove_edge(*dependency->flow_node_, *flow_node_);
  if (flow_node_->my_predecessors.empty())
    make_edge(owner_->entry_node_   , *flow_node_);
}
}