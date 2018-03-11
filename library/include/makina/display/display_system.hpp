#ifndef MAKINA_DISPLAY_DISPLAY_SYSTEM_HPP_
#define MAKINA_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <di/systems/display/display_system.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT display_system : public system, public di::display_system
{
public:
  void update(frame_timer::duration delta) override;
};
}

#endif