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

#include <makina/core/scene.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/field_load.hpp>
#include <makina/makina.hpp>

TEST_CASE("SciVis test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto input_system   = engine->get_system<mak::input_system>  ();
  const auto window         = display_system->create_opengl_window(
    "Makina (SciVis)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_swap_mode(di::opengl_swap_mode::immediate);
  window->set_resizable(true);

  const auto projection = engine->scene()->entities<mak::projection>()[0]->component<mak::projection>();
  window->on_resize.connect([projection] (const std::array<std::size_t, 2>& size)
  {
    projection->set_perspective(60.0f, float(size[0]) / float(size[1]), {0.3f, 1000.0f});
  });

  fi::initialize();
  gl::initialize();

  const auto renderer                 = engine  ->get_system<mak::renderer>();
  const auto backbuffer               = renderer->add_retained_resource("Backbuffer", mak::opengl::framebuffer::description(), mak::opengl::default_framebuffer(window));
  const auto upload_scene_render_task = mak::opengl::add_upload_scene_render_task            (renderer);
  const auto clear_render_task        = mak::opengl::add_clear_render_task                   (renderer, backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
  const auto pbr_render_task          = mak::opengl::add_physically_based_shading_render_task(renderer, backbuffer, upload_scene_render_task->data());
  const auto ui_render_task           = mak::opengl::add_ui_render_task                      (renderer, backbuffer);

  mak::field<float, 3> field;
  field.load(mak::hdf5_description<float, 3>
  {
    "C:/dev/data/pli/Human/MSA0309_s0536-0695.h5",
    "Retardation",
    "Spacing"    ,
    mak::selection<float, 3>{{0, 0, 0}, {64, 64, 64}, {1, 1, 1}}
  });

  auto entity       = engine->scene()->add_entity();
  auto metadata     = entity->add_component<mak::metadata>    ();
  auto transform    = entity->add_component<mak::transform>   ();
  auto field_render = entity->add_component<mak::field_render>();
  // field_render->field = &field;

  engine->run();
}