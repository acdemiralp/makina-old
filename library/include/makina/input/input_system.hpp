#ifndef MAKINA_INPUT_INPUT_SYSTEM_HPP_
#define MAKINA_INPUT_INPUT_SYSTEM_HPP_

#include <map>
#include <vector>

#include <di/systems/input/input_system.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
struct controller;

class MAKINA_EXPORT input_system : public system, public di::input_system
{
public:
  input_system           ();
  input_system           (const input_system&  that) = delete ;
  input_system           (      input_system&& temp) = default;
  virtual ~input_system  ()                          = default;
  input_system& operator=(const input_system&  that) = delete ;
  input_system& operator=(      input_system&& temp) = default;

  boost::signals2::signal<void(di::key)>     on_key_down  ;
  boost::signals2::signal<void(std::size_t)> on_mouse_down;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;

  std::map<di::key    , bool> key_states_      ;
  std::map<std::size_t, bool> button_states_   ;
  std::vector<controller*>    controller_cache_;
};
}

#endif