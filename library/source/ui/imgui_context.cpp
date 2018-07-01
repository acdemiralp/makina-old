#include <makina/ui/imgui_context.hpp>

namespace mak
{
ImGuiContext* imgui_context()
{
  return ImGui::GetCurrentContext();
}
}
