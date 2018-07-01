#include <catch.hpp>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/renderer/backend/vulkan/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("Vulkan test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  
  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_vulkan_window(
    mak::vulkan::context().instance->operator vk::Instance(),
    "Makina (Vulkan)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600});

  mak::vulkan::make_default_framegraph(engine.get(), window);

  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{std::string("data/model/cube/cube.obj"), true});
  mak::append_scene(model.scene.get(), engine->scene());
  
  engine->run();
}