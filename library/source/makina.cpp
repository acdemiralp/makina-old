#include <makina/makina.hpp>

#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
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
    auto entity       = scene ->add_entity();
    auto transform    = entity->add_component<mak::transform> ();
    auto projection   = entity->add_component<mak::projection>();
    auto aspect_ratio = 4.0f / 3.0f;
    transform ->set_translation(glm::vec3(0.0f, 0.0f, -10.0f));
    projection->set_perspective(210.0f / aspect_ratio, aspect_ratio, {0.01f, 1000.0f}); // Human horizontal vision is ~210 degrees.
  }
  {
    auto entity    = scene ->add_entity();
    auto transform = entity->add_component<mak::transform>();
    auto light     = entity->add_component<mak::light>    ();
    transform->set_rotation_euler(glm::vec3(50.0f, -30.0f, 0.0f));
    light->color = glm::vec3(1.0f, 0.95f, 0.83f);
  }
  engine->set_scene(std::move(scene));

  return engine;
}
}