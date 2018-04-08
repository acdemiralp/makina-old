#include <catch.hpp>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef MemoryBarrier
extern "C"
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}
#endif

#include <fi/free_image.hpp>

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/display/display_system.hpp>
#include <makina/renderer/backend/vulkan/render_tasks.hpp>
#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/ui/ui_system.hpp>
#include <makina/makina.hpp>

TEST_CASE("Vulkan test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  engine->remove_system<mak::ui_system>(); // UI system needs GPU texture allocators to be available.
  
  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_vulkan_window(
    mak::vulkan::context().instance->operator vk::Instance(),
    "Makina (Vulkan)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600});
  mak::vulkan::context().create_window_swapchains(display_system->vulkan_windows());
  
  fi::initialize();

  const auto renderer                      = engine->get_system<mak::renderer>();
  const auto upload_scene_task             = mak::vulkan::add_upload_scene_render_task            (renderer);
  const auto skybox_task                   = mak::vulkan::add_test_render_task                    (renderer);
  const auto physically_based_shading_task = mak::vulkan::add_physically_based_shading_render_task(renderer, nullptr, upload_scene_task->data());
  const auto ui_task                       = mak::vulkan::add_ui_render_task                      (renderer, nullptr);
  const auto present_render_task           = mak::vulkan::add_present_render_task                 (renderer);
 
  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{std::string("data/model/setesh/setesh.obj"), true});
  auto  entity = engine->scene()->copy_entity(model.scene->entities()[1]);
  
  engine->run();
}