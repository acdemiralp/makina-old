#include <makina/renderer/backend/opengl/render_tasks/create_hmd_textures_render_task.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <di/systems/vr/hmd.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<create_hmd_textures_task_data>* add_create_hmd_textures_render_task(renderer* framegraph, di::hmd* hmd)
{
  const std::array<GLsizei, 2> size {hmd->recommended_render_target_size()[0], hmd->recommended_render_target_size()[1]};
  const auto left  = framegraph->add_retained_resource<framebuffer::description, framebuffer>("Left  Eye Framebuffer", framebuffer::description{size});
  const auto right = framegraph->add_retained_resource<framebuffer::description, framebuffer>("Right Eye Framebuffer", framebuffer::description{size});

  auto render_task = framegraph->add_render_task<create_hmd_textures_task_data>(
    "Submit HMD Render Pass",
    [&] (      create_hmd_textures_task_data& data, fg::render_task_builder& builder)
    {
      data.left  = builder.write(left );
      data.right = builder.write(right);
    },
    [=] (const create_hmd_textures_task_data& data)
    {

    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}