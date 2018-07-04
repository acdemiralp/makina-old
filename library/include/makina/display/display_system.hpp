#ifndef MAKINA_DISPLAY_DISPLAY_SYSTEM_HPP
#define MAKINA_DISPLAY_DISPLAY_SYSTEM_HPP

#include <di/systems/display/display_system.hpp>
#include <di/systems/display/opengl_window.hpp>
#include <di/systems/display/vulkan_window.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT display_system : public system, public di::display_system
{
public:
  std::vector<di::opengl_window*> opengl_windows() const;
  std::vector<di::vulkan_window*> vulkan_windows() const;

protected:
  void update(frame_timer::duration delta, scene* scene) override;
};
}

#endif