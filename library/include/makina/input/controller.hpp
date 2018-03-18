#ifndef MAKINA_INPUT_CONTROLLER_HPP_
#define MAKINA_INPUT_CONTROLLER_HPP_

#include <array>
#include <cstddef>
#include <functional>

#include <di/systems/input/key.hpp>

#include <makina/core/frame_timer.hpp>
#include <makina/export.hpp>

namespace mak
{
class transform;

struct MAKINA_EXPORT controller
{
  std::function<void(transform*, frame_timer::duration, di::key)>                    on_key_press    ;
  std::function<void(transform*, frame_timer::duration, di::key)>                    on_key_down     ;
  std::function<void(transform*, frame_timer::duration, di::key)>                    on_key_release  ;
  std::function<void(transform*, frame_timer::duration, std::size_t)>                on_mouse_press  ;
  std::function<void(transform*, frame_timer::duration, std::size_t)>                on_mouse_down   ;
  std::function<void(transform*, frame_timer::duration, std::size_t)>                on_mouse_release;
  std::function<void(transform*, frame_timer::duration, std::array<std::size_t, 2>)> on_mouse_move   ;
  std::function<void(transform*, frame_timer::duration, std::array<std::size_t, 2>)> on_mouse_wheel  ;
};
}

#endif