#ifndef MAKINA_UI_UI_SYSTEM_HPP_
#define MAKINA_UI_UI_SYSTEM_HPP_

#include <imgui.h>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT ui_system : public system
{
public:
  ui_system           ()                       = default;
  ui_system           (const ui_system&  that) = default;
  ui_system           (      ui_system&& temp) = default;
  virtual ~ui_system  ()                       = default;
  ui_system& operator=(const ui_system&  that) = default;
  ui_system& operator=(      ui_system&& temp) = default;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;
};
}

#endif