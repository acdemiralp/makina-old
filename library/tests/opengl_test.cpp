#include <catch.hpp>

#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("OpenGL test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (OpenGL)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_resizable(true);
  window->on_resize.connect([&engine] (const std::array<std::size_t, 2>& size)
  {
    engine->scene()->entities<mak::projection>()[0]->component<mak::projection>()->set_perspective(60.0f, float(size[0]) / float(size[1]), {0.3f, 1000.0f});
  });

  mak::opengl::make_default_framegraph(engine.get(), window);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{"data/model/nightsaber/nightsaber.fbx", true});
  for(auto i = 0; i < 8; ++i)
    mak::append_scene(model.scene.get(), engine->scene());
  
  mak::random_number_generator<std::uniform_real_distribution<float>> rng(0.0f, 5.0f);
  for (auto entity : engine->scene()->entities<mak::mesh_render>())
  {
    auto transform = entity->component<mak::transform>();
    transform->set_translation(glm::vec3(rng.generate(), 0.0f, rng.generate()));
    transform->set_scale      (glm::vec3(0.01f));
  }

  engine->run();
}