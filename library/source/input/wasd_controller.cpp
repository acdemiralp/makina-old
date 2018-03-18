#include <makina/input/wasd_controller.hpp>

#include <glm/vec3.hpp>

#include <makina/renderer/transform.hpp>

namespace mak
{
controller make_wasd_controller()
{
  controller wasd_controller;
  wasd_controller.on_key_press     = [ ] (transform* transform, di::key key)
  {
    if(key.code == di::key_code::w) transform->translate(glm::vec3(-1, 0, 0));
    if(key.code == di::key_code::a) transform->translate(glm::vec3( 1, 0, 0));
    if(key.code == di::key_code::s) transform->translate(glm::vec3( 1, 0, 0));
    if(key.code == di::key_code::d) transform->translate(glm::vec3(-1, 0, 0));
  };
  wasd_controller.on_key_release   = [ ] (transform* transform, di::key key)
  {

  };
  wasd_controller.on_mouse_press   = [ ] (transform* transform, std::size_t button)
  {

  };
  wasd_controller.on_mouse_release = [ ] (transform* transform, std::size_t button)
  {

  };
  wasd_controller.on_mouse_move    = [ ] (transform* transform, std::array<std::size_t, 2> delta)
  {

  };
  wasd_controller.on_mouse_wheel   = [ ] (transform* transform, std::array<std::size_t, 2> delta)
  {

  };
  return wasd_controller;
}
}
