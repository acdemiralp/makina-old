#include <makina/input/wasd_controller.hpp>

#include <di/systems/input/mouse.hpp>
#include <glm/vec3.hpp>

#include <makina/renderer/transform.hpp>

namespace mak
{
controller make_wasd_controller(const float move_sensitivity, const float look_sensitivity)
{
  controller wasd_controller;
  wasd_controller.on_key_down   = [=] (transform* transform, di::key key)
  {
    if(key.code == di::key_code::w) transform->translate(transform->forward() *  move_sensitivity);
    if(key.code == di::key_code::s) transform->translate(transform->forward() * -move_sensitivity);
    if(key.code == di::key_code::a) transform->translate(transform->right  () * -move_sensitivity);
    if(key.code == di::key_code::d) transform->translate(transform->right  () *  move_sensitivity);
  };
  wasd_controller.on_mouse_move = [=] (transform* transform, std::array<std::size_t, 2> value)
  {
    di::mouse::set_captured(true );
    di::mouse::set_visible (false);
    transform->rotate_euler(glm::vec3(0.0f, value[0] * look_sensitivity, 0.0f));
    transform->rotate_euler(glm::vec3(value[1] * look_sensitivity, 0.0f, 0.0f));
  };
  return wasd_controller;
}
}
