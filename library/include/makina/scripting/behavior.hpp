#ifndef MAKINA_SCRIPTING_BEHAVIOR_HPP
#define MAKINA_SCRIPTING_BEHAVIOR_HPP

#include <functional>

#include <makina/core/frame_timer.hpp>
#include <makina/core/scene.hpp>
#include <makina/export.hpp>

namespace mak
{
class scripting_system;

class MAKINA_EXPORT behavior
{
public:
  explicit behavior  (
    const std::function<void(scene*, entity*)>                        on_prepare,
    const std::function<void(frame_timer::duration, scene*, entity*)> on_update )
    : on_prepare_(on_prepare), on_update_(on_update)
  {
    
  }
  behavior           (const behavior&  that) = default;
  behavior           (      behavior&& temp) = default;
  virtual ~behavior  ()                      = default;
  behavior& operator=(const behavior&  that) = default;
  behavior& operator=(      behavior&& temp) = default;

protected:
  friend scripting_system;

  std::function<void(scene*, entity*)>                        on_prepare_;
  std::function<void(frame_timer::duration, scene*, entity*)> on_update_ ;
};
}

#endif