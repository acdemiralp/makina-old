#ifndef MAKINA_SCRIPTING_BEHAVIOR_HPP_
#define MAKINA_SCRIPTING_BEHAVIOR_HPP_

#include <memory>
#include <vector>

#include <makina/core/frame_timer.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT behavior
{
public:
  behavior           ()                      = default;
  behavior           (const behavior&  that) = default;
  behavior           (      behavior&& temp) = default;
  virtual ~behavior  ()                      = default;
  behavior& operator=(const behavior&  that) = default;
  behavior& operator=(      behavior&& temp) = default;

  virtual void on_prepare()                            {}
  virtual void on_update (frame_timer::duration delta) {}
  
protected:
  // TODO: Each behavior may access its entity and its other components (as well as the scene? and the engine?).
};

// Each entity may have a vector of behaviors.
using behaviors = std::vector<std::unique_ptr<behavior>>;
}

#endif