#ifndef MAKINA_INPUT_CONTROLLER_HPP
#define MAKINA_INPUT_CONTROLLER_HPP

#include <array>
#include <cstddef>
#include <functional>

#include <di/systems/input/key.hpp>

#include <makina/export.hpp>

namespace mak
{
class transform;

struct MAKINA_EXPORT controller
{
  std::function<void(transform*, di::key)>                     on_key_press    ;
  std::function<void(transform*, di::key)>                     on_key_down     ;
  std::function<void(transform*, di::key)>                     on_key_release  ;
  std::function<void(transform*, std::size_t)>                 on_mouse_press  ;
  std::function<void(transform*, std::size_t)>                 on_mouse_down   ;
  std::function<void(transform*, std::size_t)>                 on_mouse_release;
  std::function<void(transform*, std::array<std::int32_t, 2>)> on_mouse_move   ;
  std::function<void(transform*, std::array<std::int32_t, 2>)> on_mouse_wheel  ;
};
}

#endif