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

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/display/display_system.hpp>
#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/audio_clip_load.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/scripting/behavior.hpp>
#include <makina/makina.hpp>

TEST_CASE("OpenGL test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (OpenGL)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_swap_mode(di::opengl_swap_mode::vertical_sync);

  fi::initialize();
  gl::initialize();

  const auto renderer   = engine->get_system<mak::renderer>();
  const auto backbuffer = renderer->add_retained_resource("Backbuffer", mak::opengl::framebuffer::description(), mak::opengl::default_framebuffer());
  const auto upload_scene_render_task = mak::opengl::add_upload_scene_render_task            (renderer);
  const auto clear_render_task        = mak::opengl::add_clear_render_task                   (renderer, backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
  const auto skybox_render_task       = mak::opengl::add_test_render_task                    (renderer, backbuffer);
  const auto pbr_render_task          = mak::opengl::add_physically_based_shading_render_task(renderer, backbuffer, upload_scene_render_task->data());
  const auto ui_render_task           = mak::opengl::add_ui_render_task                      (renderer, backbuffer);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{"data/model/cube/cube.obj", true});
  
  auto& audio_clips = mak::registry->get<mak::audio_clip>().storage();
  auto& audio_clip  = audio_clips.emplace_back(mak::audio_clip::description{"data/audio/test/test.mp3", true});

  auto behaviors = engine->scene()->entities()[0]->add_component<mak::behaviors>();
  behaviors->push_back(std::make_shared<mak::behavior>(
    [] (mak::scene* scene, mak::entity* entity)
    {
    
    }, 
    [] (mak::frame_timer::duration duration, mak::scene* scene, mak::entity* entity)
    {
      entity->component<mak::transform>()->translate({0.0f, -0.01f, 0.0f});
    }));

  std::random_device                    random_device   ;
  std::mt19937                          mersenne_twister(random_device());
  std::uniform_real_distribution<float> distribution    (-0.25f, 0.25f);
  for(auto i = 0; i < 8; ++i)
  {
    auto entity    = engine->scene()->copy_entity(model.scene->entities()[1]); // TODO: Preserve transform hierarchy when appending / copying.
    auto transform = entity->component<mak::transform>();
    transform->set_translation(glm::vec3(distribution(mersenne_twister), distribution(mersenne_twister), distribution(mersenne_twister)));

    if(i == 0)
    {
      entity->remove_component<mak::rigidbody>();

      auto audio_source  = entity->add_component<mak::audio_source>();
      audio_source->set_clip   (&audio_clip);
      audio_source->set_looping(true);
    }
  }
  
  engine->run();
}