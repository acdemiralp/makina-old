#include <catch.hpp>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
extern "C"
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}
#endif

#include <fi/free_image.hpp>
#include <gl/all.hpp>
#include <ra/load.hpp>

#include <makina/core/registry.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/makina.hpp>

TEST_CASE("Makina test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  display_system->create_opengl_window(
    "Makina", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{640, 480}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});

  const auto input_system = engine->get_system<mak::input_system>();
  input_system->on_quit.connect(std::bind(&mak::engine::stop, engine.get()));

  fi::initialize();
  gl::initialize();

  const auto renderer   = engine->get_system<mak::renderer>();
  const auto backbuffer = renderer->add_retained_resource("Backbuffer", mak::framebuffer::description(), mak::default_framebuffer());
  const auto upload_scene_render_task = add_upload_scene_render_task(renderer);
  const auto clear_render_task        = add_clear_render_task       (renderer, backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
  const auto phong_render_task        = add_phong_render_task       (renderer, backbuffer, upload_scene_render_task->data());
  upload_scene_render_task->set_cull_immune(true);
  clear_render_task       ->set_cull_immune(true);
  phong_render_task       ->set_cull_immune(true);

  auto& models = mak::registry->get<mak::model>();
  auto& model  = models.storage().emplace_back();
  ra::load(std::string("data/model/setesh/setesh.obj"), &model);
  engine->scene()->append(*model.scene);

  engine->run();
}