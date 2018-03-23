#include <makina/makina.hpp>

#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/input/wasd_controller.hpp>
#include <makina/renderer/renderer.hpp>

namespace mak
{
std::unique_ptr<engine> make_default_engine()
{
  auto engine = std::make_unique<mak::engine>();
  engine->add_system<display_system>();
  engine->add_system<input_system>  ();
  engine->add_system<renderer>      ();

  auto scene = std::make_unique<mak::scene>();
  {
    auto entity          = scene ->add_entity();
    auto transform       = entity->add_component<mak::transform> ();
    auto projection      = entity->add_component<mak::projection>();
    auto controller      = entity->add_component<mak::controller>(make_wasd_controller());
    transform ->set_translation(glm::vec3(0.0f, 1.0f, -10.0f));
    projection->set_perspective(60.0f, 4.0f / 3.0f, {0.3f, 1000.0f});
  }
  {
    auto entity          = scene ->add_entity();
    auto transform       = entity->add_component<mak::transform>();
    auto light           = entity->add_component<mak::light>    ();
    light    ->type      = light::type::ambient;
    light    ->color     = glm::vec3(1.0f, 0.95f, 0.83f);
    light    ->intensity = 0.1f;
  }
  {
    auto entity          = scene ->add_entity();
    auto transform       = entity->add_component<mak::transform>();
    auto light           = entity->add_component<mak::light>    ();
    transform->set_rotation_euler(glm::vec3(50.0f, -30.0f, 0.0f));
    light    ->type      = light::type::directional;
    light    ->color     = glm::vec3(1.0f, 0.95f, 0.83f);
  }

  engine->set_scene(std::move(scene));

  return engine;
}
}