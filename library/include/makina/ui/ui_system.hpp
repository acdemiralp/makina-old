#ifndef MAKINA_UI_UI_SYSTEM_HPP_
#define MAKINA_UI_UI_SYSTEM_HPP_

#include <array>

#include <di/systems/input/mouse.hpp>
#include <imgui.h>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class display_system;
class input_system;

class MAKINA_EXPORT ui_system : public system
{
public:
  explicit ui_system  (display_system* display_system, input_system* input_system);
  ui_system           (const ui_system&  that) = default;
  ui_system           (      ui_system&& temp) = default;
  virtual ~ui_system  ()                       = default;
  ui_system& operator=(const ui_system&  that) = default;
  ui_system& operator=(      ui_system&& temp) = default;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;

  display_system*                                  display_system_;
  input_system*                                    input_system_  ;
  ImGuiContext*                                    context_       ;
  std::array<SDL_Cursor*, ImGuiMouseCursor_Count_> cursors_       ;
};
}

#endif