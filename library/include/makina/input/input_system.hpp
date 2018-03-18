#ifndef MAKINA_INPUT_INPUT_SYSTEM_HPP_
#define MAKINA_INPUT_INPUT_SYSTEM_HPP_

#include <vector>

#include <di/systems/input/input_system.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
struct controller;

class MAKINA_EXPORT input_system : public system, public di::input_system
{
protected:
  void update(frame_timer::duration delta, scene* scene) override;

  std::vector<controller*> controller_cache_;
};
}

#endif