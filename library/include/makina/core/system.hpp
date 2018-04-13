#ifndef MAKINA_CORE_SYSTEM_HPP_
#define MAKINA_CORE_SYSTEM_HPP_

#include <optional>

#include <tbb/flow_graph.h>

#include <makina/core/flow_node.hpp>
#include <makina/core/frame_timer.hpp>
#include <makina/core/scene.hpp>
#include <makina/export.hpp>

namespace mak
{
class engine;

// A system is anything that updates regularly. 
// All systems are run on the main thread by default, but may be offloaded to another thread via set_async.
class MAKINA_EXPORT system
{
public:
  virtual ~system               ()                                          = default;
  virtual void prepare          (                             scene* scene) { }
  virtual void update           (frame_timer::duration delta, scene* scene) = 0;
                                
  void         set_async        (const bool async)
  {
    async 
      ? flow_node_.emplace(*flow_graph_, 64, [&] (const flow_node_data& data)
      {
        data.prepare ? prepare(data.scene) : update(data.duration, data.scene);
        return data;
      }) 
      : flow_node_ = std::nullopt;
  }
  bool         async            () const
  {
    return flow_node_.has_value();
  }

  void         add_dependency   (system* dependency)
  {
    remove_edge(source_node_           , *flow_node_);
    make_edge  (*dependency->flow_node_, *flow_node_);
  }
  void         remove_dependency(system* dependency)
  {
    remove_edge(*dependency->flow_node_, *flow_node_);
    make_edge  (source_node_           , *flow_node_); // Only if last.
  }

protected:
  friend engine;

  void         set_metadata     (tbb::flow::graph* flow_graph, flow_node* source_node)
  {
    flow_graph_ = flow_graph;
    make_edge(*source_node, *flow_node_);
  }

  tbb::flow::graph*        flow_graph_ = nullptr     ;
  std::optional<flow_node> flow_node_  = std::nullopt; // Runs on main thread when an async flow node is not defined.
};
}

#endif