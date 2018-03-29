#include <makina/display/display_system.hpp>

namespace mak
{
std::vector<di::opengl_window*> display_system::opengl_windows() const
{
  std::vector<di::opengl_window*> opengl_windows;
  for (auto window : windows())
  {
    auto opengl_window = dynamic_cast<di::opengl_window*>(window);
    if  (opengl_window)
      opengl_windows.push_back(opengl_window);
  }
  return opengl_windows;
}
std::vector<di::vulkan_window*> display_system::vulkan_windows() const
{
  std::vector<di::vulkan_window*> vulkan_windows;
  for (auto window : windows())
  {
    auto vulkan_window = dynamic_cast<di::vulkan_window*>(window);
    if (vulkan_window)
      vulkan_windows.push_back(vulkan_window);
  }
  return vulkan_windows;
}

  void display_system::update(frame_timer::duration delta, scene* scene)
{
  tick();
}
}
