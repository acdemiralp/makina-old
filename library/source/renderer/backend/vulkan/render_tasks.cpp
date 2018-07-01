#include <makina/renderer/backend/vulkan/render_tasks.hpp>

#include <makina/core/engine.hpp>
#include <makina/display/display_system.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>

namespace mak
{
void vulkan::make_default_framegraph(engine* engine, di::vulkan_window* window)
{
  context().create_window_swapchains(engine->get_system<display_system>()->vulkan_windows());

  const auto renderer                      = engine->get_system<mak::renderer>();
  const auto target_swapchain              = std::find_if(context().window_swapchains.begin(), context().window_swapchains.end(), [window] (const window_swapchain& iteratee) { return iteratee.window == window; });
  const auto backbuffer                    = renderer->add_retained_resource("Backbuffer", framebuffer_description{}, &target_swapchain->framebuffer);
  const auto upload_scene_task             = add_upload_scene_render_task            (renderer);
  const auto skybox_task                   = add_test_render_task                    (renderer);
  const auto physically_based_shading_task = add_physically_based_shading_render_task(renderer, backbuffer, upload_scene_task->data());
  const auto ui_task                       = add_ui_render_task                      (renderer, backbuffer);
  const auto present_render_task           = add_present_render_task                 (renderer, backbuffer);
}
}
