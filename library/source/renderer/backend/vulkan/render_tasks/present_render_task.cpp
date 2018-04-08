#include <makina/renderer/backend/vulkan/render_tasks/present_render_task.hpp>

#include <makina/renderer/backend/vulkan/context.hpp>

namespace mak
{
namespace vulkan
{
fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<present_task_data>(
    "Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const present_task_data& data)
    {
      vulkan::context().present_window_swapchains();
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}
