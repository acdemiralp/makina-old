#include <makina/renderer/backend/opengl/render_tasks/submit_hmd_textures_render_task.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <di/systems/vr/hmd.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<submit_hmd_textures_task_data>* add_submit_hmd_textures_render_task(renderer* framegraph, di::hmd* hmd, const create_hmd_textures_task_data& texture_data)
{
  auto render_task = framegraph->add_render_task<submit_hmd_textures_task_data>(
    "Submit HMD Render Pass",
    [&] (      submit_hmd_textures_task_data& data, fg::render_task_builder& builder)
    {
      data.left  = builder.read(texture_data.left );
      data.right = builder.read(texture_data.right);
    },
    [=] (const submit_hmd_textures_task_data& data)
    {
      hmd->submit(di::eye::left , data.left ->actual()->id());
      hmd->submit(di::eye::right, data.right->actual()->id());
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}