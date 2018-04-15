#include <makina/makina.hpp>

#include <makina/audio/audio_system.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/input/wasd_controller.hpp>
#include <makina/physics/physics_system.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/scripting/scripting_system.hpp>
#include <makina/ui/ui_system.hpp>

namespace mak
{
std::unique_ptr<engine> make_default_engine()
{
  auto engine           = std::make_unique<mak::engine>            ();
  auto display_system   = engine->add_system<mak::display_system>  ();
  auto input_system     = engine->add_system<mak::input_system>    ();
  auto ui_system        = engine->add_system<mak::ui_system>       (display_system, input_system);
  auto scripting_system = engine->add_system<mak::scripting_system>();
  auto audio_system     = engine->add_system<mak::audio_system>    ();
  auto physics_system   = engine->add_system<mak::physics_system>  ();
  auto renderer         = engine->add_system<mak::renderer>        ();
  input_system    ->set_async(true);
  scripting_system->set_async(true);
  audio_system    ->set_async(true);
  physics_system  ->set_async(true);

  input_system->on_quit.connect(std::bind(&engine::stop, engine.get()));

  auto scene = std::make_unique<mak::scene>();
  {
    auto entity          = scene ->add_entity();
    auto transform       = entity->add_component<mak::transform>     ();
    auto projection      = entity->add_component<mak::projection>    ();
    auto audio_listener  = entity->add_component<mak::audio_listener>();
    auto controller      = entity->add_component<mak::controller>    (make_wasd_controller());
    transform ->set_translation(glm::vec3(0.0f, 1.0f, -10.0f));
    projection->set_perspective(60.0f, 4.0f / 3.0f, {0.3f, 1000.0f});
  }
  {
    auto entity          = scene ->add_entity();
    auto transform       = entity->add_component<mak::transform>();
    auto light           = entity->add_component<mak::light>    ();
    light    ->type      = light::type::ambient;
    light    ->color     = glm::vec3(1.0f, 0.95f, 0.83f);
    light    ->intensity = 0.001f;
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