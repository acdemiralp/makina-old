#ifndef MAKINA_CORE_FLOW_NODE_HPP_
#define MAKINA_CORE_FLOW_NODE_HPP_

#include <tbb/flow_graph.h>

#include <makina/core/frame_timer.hpp>
#include <makina/core/scene.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT flow_node_data
{
  bool                  prepare  = false  ;
  scene*                scene    = nullptr;
  frame_timer::duration duration = frame_timer::duration();
};

using entry_node = tbb::flow::broadcast_node<flow_node_data>;
using flow_node  = tbb::flow::function_node <flow_node_data, flow_node_data>;
}

#endif