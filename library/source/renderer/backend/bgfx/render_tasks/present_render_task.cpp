#include <makina/renderer/backend/bgfx/render_tasks/present_render_task.hpp>

#include <bgfx/bgfx.h>
#include <fg/render_task_builder.hpp>

namespace mak
{
namespace bgfx
{
fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<present_task_data>(
    "BGFX Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {
    
    },
    [ ] (const present_task_data& data)
    {
      ::bgfx::frame() ;
      ::bgfx::touch(0);
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}