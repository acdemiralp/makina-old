#ifndef MAKINA_CORE_SYSTEM_HPP
#define MAKINA_CORE_SYSTEM_HPP

#include <optional>

#include <makina/core/flow_node.hpp>
#include <makina/core/frame_timer.hpp>
#include <makina/core/scene.hpp>
#include <makina/export.hpp>

namespace mak
{
class engine;

// A system is anything that updates regularly. 
// All systems are run on the main thread sequentially by default.
// However they may be offloaded to another thread via set_async.
class MAKINA_EXPORT system
{
public:
  virtual ~system     ()                                          = default;
  virtual void prepare(                             scene* scene) { }
  virtual void update (frame_timer::duration delta, scene* scene) = 0;
                                           
  void set_async        (const bool async);
  bool async            () const;
                                                       
  void add_dependency   (system* dependency);
  void remove_dependency(system* dependency);

protected:
  friend engine;

  engine*                  owner_     = nullptr     ;
  std::optional<flow_node> flow_node_ = std::nullopt; // Runs on main thread when an async flow node is not defined.
};
}

#endif