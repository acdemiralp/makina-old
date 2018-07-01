#include <makina/renderer/backend/ospray/render_tasks/clear_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<clear_task_data>* add_clear_render_task(renderer* renderer, framebuffer_resource* target)
{
  return renderer->add_render_task<clear_task_data>(
    "Clear Pass",
    [&] (      clear_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [=] (const clear_task_data& data)
    {
      data.target->actual()->clear(OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM);
    });
}
}
}