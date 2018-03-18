#include <makina/input/wasd_controller.hpp>

#include <glm/vec3.hpp>

#include <makina/renderer/transform.hpp>

namespace mak
{
controller make_wasd_controller(const float move_sensitivity, const float look_sensitivity)
{
  controller wasd_controller;
  wasd_controller.on_key_down   = [=] (transform* transform, frame_timer::duration delta, di::key key)
  {
    if(key.code == di::key_code::w) transform->translate(glm::vec3(0, 0, -move_sensitivity * delta.count()));
    if(key.code == di::key_code::s) transform->translate(glm::vec3(0, 0,  move_sensitivity * delta.count()));
    if(key.code == di::key_code::a) transform->translate(glm::vec3(-move_sensitivity * delta.count(), 0, 0));
    if(key.code == di::key_code::d) transform->translate(glm::vec3( move_sensitivity * delta.count(), 0, 0));
  };
  wasd_controller.on_mouse_move = [=] (transform* transform, frame_timer::duration delta, std::array<std::size_t, 2> value)
  {
    transform->rotate_euler(glm::vec3(0.0f, value[0] * delta.count(), 0.0f));
    transform->rotate_euler(glm::vec3(value[1] * delta.count(), 0.0f, 0.0f));
  };
  return wasd_controller;
}
}
