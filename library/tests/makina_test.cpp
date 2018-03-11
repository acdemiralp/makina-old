#include <catch.hpp>

#define NOMINMAX
#include <gl/all.hpp>

#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/makina.hpp>

TEST_CASE("Makina test.", "[makina]")
{
  struct render_task_data { };

  gl::initialize();

  auto engine = mak::make_default_engine();
  engine->get_system<mak::display_system>()->create_opengl_window("Makina", std::array<std::size_t, 2>{32, 32}, std::array<std::size_t, 2>{640, 480}, di::opengl_context_settings{di::opengl_profile::core, 4, 2});
  engine->get_system<mak::input_system>  ()->on_quit.connect(std::bind(&mak::engine::stop, engine.get()));
  engine->get_system<mak::renderer>      ()->add_render_task<render_task_data>(
    "Render Task",
    [&] (render_task_data& data, fg::render_task_builder& builder)
    {
      gl::set_clear_color       ({0.0F, 0.0F, 0.0F, 0.0F});
      gl::set_depth_test_enabled(true);
    },
    [=] (const render_task_data& data)
    {
      gl::set_viewport({0, 0}, {640, 480});
      gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
  engine->run();
}