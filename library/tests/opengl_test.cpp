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

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/makina.hpp>

TEST_CASE("OpenGL test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto input_system   = engine->get_system<mak::input_system>  ();
  const auto window         = display_system->create_opengl_window(
    "Makina (OpenGL)", 
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

  const auto renderer   = engine->get_system<mak::renderer>();
  const auto backbuffer = renderer->add_retained_resource("Backbuffer", mak::opengl::framebuffer::description(), mak::opengl::default_framebuffer(window));
  const auto upload_scene_render_task       = mak::opengl::add_upload_scene_render_task            (renderer);
  //const auto skeletal_animation_render_task = mak::opengl::add_skeletal_animation_render_task      (renderer, upload_scene_render_task->data());
  
  auto upload_scene_task_data               = upload_scene_render_task      ->data();
  //auto skeletal_animation_task_data         = skeletal_animation_render_task->data();
  //upload_scene_task_data.vertices           = skeletal_animation_task_data.transformed_vertices;
  //upload_scene_task_data.normals            = skeletal_animation_task_data.transformed_normals ;
  
  const auto clear_render_task              = mak::opengl::add_clear_render_task                   (renderer,               backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
  const auto pbr_render_task                = mak::opengl::add_physically_based_shading_render_task(renderer,               backbuffer, upload_scene_task_data);
  const auto immediate_render_task          = mak::opengl::add_immediate_render_task               (renderer, input_system, backbuffer, upload_scene_task_data);
  const auto ui_render_task                 = mak::opengl::add_ui_render_task                      (renderer,               backbuffer);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{"data/model/nightsaber/nightsaber.fbx", true});
  mak::append_scene(model.scene.get(), engine->scene());
  
  engine->scene()->entities<mak::mesh_render>()[0]->component<mak::transform>()->set_scale(glm::vec3(0.1f), true);
  engine->scene()->entities<mak::animator>   ()[0]->component<mak::animator> ()->play = true;

  engine->run();
}