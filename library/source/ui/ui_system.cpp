#include <makina/ui/ui_system.hpp>

namespace mak
{
void ui_system::prepare(                             scene* scene)
{
  ImGui::CreateContext();
  auto& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui::Text("Makina UI");

}
void ui_system::update (frame_timer::duration delta, scene* scene)
{

}
}
