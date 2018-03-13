#include <catch.hpp>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <fi/free_image.hpp>
#include <gl/all.hpp>
#include <ra/load.hpp>

#include <makina/core/registry.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl_render_tasks.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/makina.hpp>

TEST_CASE("Makina test.", "[makina]")
{
  struct render_task_data { };

  fi::initialize();
  gl::initialize();

  auto engine = mak::make_default_engine();

  engine->get_system<mak::display_system>()->create_opengl_window(
    "Makina", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{640, 480}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 2});

  engine->get_system<mak::input_system>()->on_quit.connect(std::bind(&mak::engine::stop, engine.get()));
  
  auto renderer = engine->get_system<mak::renderer>();
  mak::add_clear_render_task(renderer, {0.0F, 0.0F, 0.0F, 1.0F});
  mak::add_phong_render_task(renderer);

  auto& models = mak::registry->get<mak::model>();
  models.storage().emplace_back();
  auto& model  = models.storage().back();
  ra::load(std::string("data/model/setesh/setesh.obj"), &model);
  engine->set_scene(std::make_unique<mak::scene>(*model.scene));

  engine->run();
}