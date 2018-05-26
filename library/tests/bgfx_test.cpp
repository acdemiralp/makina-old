#include <catch.hpp>

#include <makina/renderer/backend/bgfx/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("BGFX test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  
  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (BGFX)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600},
    di::opengl_context_settings{di::opengl_profile::compatibility, 4, 5});
  window->set_resizable(true);

  mak::bgfx::make_default_framegraph(engine.get(), window);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{std::string("data/model/cube/cube.obj"), true});
  engine->scene()->copy_entity(model.scene->entities()[1]);
  
  engine->run();
}