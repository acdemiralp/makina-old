#ifndef MAKINA_INPUT_INPUT_SYSTEM_HPP_
#define MAKINA_INPUT_INPUT_SYSTEM_HPP_

#include <di/systems/input/input_system.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT input_system : public system, public di::input_system
{
public:
  void update(frame_timer::duration delta, scene* scene) override;
};
}

#endif