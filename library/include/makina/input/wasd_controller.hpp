#ifndef MAKINA_INPUT_WASD_CONTROLLER_HPP
#define MAKINA_INPUT_WASD_CONTROLLER_HPP

#include <makina/input/controller.hpp>

namespace mak
{
controller make_wasd_controller(const float move_sensitivity = 1.0f, const float look_sensitivity = 0.25f);
}

#endif