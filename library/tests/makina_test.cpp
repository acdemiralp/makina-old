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

#include <random>

#include <fi/free_image.hpp>
#include <gl/all.hpp>
#include <ra/load.hpp>
#include <imgui.h>

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/makina.hpp>

TEST_CASE("Makina test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  auto window = display_system->create_opengl_window(
    "Makina", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});

  fi::initialize();
  gl::initialize();

  const auto input_system = engine->get_system<mak::input_system>();
  input_system->on_quit.connect(std::bind(&mak::engine::stop, engine.get()));

  const auto renderer   = engine->get_system<mak::renderer>();
  const auto backbuffer = renderer->add_retained_resource("Backbuffer", mak::framebuffer::description(), mak::default_framebuffer());
  const auto upload_scene_render_task = add_upload_scene_render_task            (renderer);
  const auto clear_render_task        = add_clear_render_task                   (renderer, backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
  const auto pbr_render_task          = add_physically_based_shading_render_task(renderer, backbuffer, upload_scene_render_task->data());
  const auto ui_render_task           = mak::add_ui_render_task                 (renderer, backbuffer);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{std::string("data/model/cube/cube.obj"), true});

  ImGui::Text("Makina UI");

  std::random_device                    rd  ;
  std::mt19937                          mt  (rd());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
  for(auto i = 0; i < 64; ++i)
  {
    auto entity    = engine->scene()->copy_entity(model.scene->entities()[1]); // TODO: Preserve transform hierarchy when appending / copying.
    auto transform = entity->component<mak::transform>();
    transform->set_translation(glm::vec3(dist(mt), dist(mt), dist(mt)));
  }
  
  //{
  //  auto entity        = engine->scene()->add_entity();
  //  auto transform     = entity->add_component<mak::transform>();
  //  auto light         = entity->add_component<mak::light>    ();
  //  light->type        = mak::light::type::ambient;
  //  light->color       = glm::vec3(1.0f, 0.0f, 0.0f);
  //}
  //{
  //  auto entity        = engine->scene()->add_entity();
  //  auto transform     = entity->add_component<mak::transform>();
  //  auto light         = entity->add_component<mak::light>    ();
  //  light->type        = mak::light::type::directional;
  //  light->color       = glm::vec3(0.0f, 1.0f, 0.0f);
  //}
  //{
  //  auto entity        = engine->scene()->add_entity();
  //  auto transform     = entity->add_component<mak::transform>();
  //  auto light         = entity->add_component<mak::light>    ();
  //  transform->set_translation(glm::vec3(0.0f, 0.0f, 50.0f));
  //  light->type        = mak::light::type::point;
  //  light->color       = glm::vec3(0.0f, 0.0f, 1.0f);
  //}
  //{
  //  auto entity        = engine->scene()->add_entity();
  //  auto transform     = entity->add_component<mak::transform>();
  //  auto light         = entity->add_component<mak::light>    ();
  //  transform->set_translation(glm::vec3(0.0f, 0.0f, -10.0f));
  //  light->type        = mak::light::type::spot;
  //  light->spot_angles = {5.0f, 15.0f};
  //  light->color       = glm::vec3(1.0f, 1.0f, 1.0f);
  //}

  engine->run();
}