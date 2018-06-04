#include <makina/renderer/backend/ospray/render_tasks/create_hmd_textures_render_task.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <di/systems/vr/hmd.hpp>
#include <ospray/ospray_cpp.h>

namespace mak
{
namespace ospray
{
fg::render_task<create_hmd_textures_task_data>* add_create_hmd_textures_render_task(renderer* renderer, di::hmd* hmd)
{
  const ospcommon::vec2i size {hmd->recommended_render_target_size()[0], hmd->recommended_render_target_size()[1]};
  const auto left  = renderer->add_retained_resource<framebuffer_description, ::ospray::cpp::FrameBuffer>(
    "Left Eye Framebuffer" , framebuffer_description { size, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM });
  const auto right = renderer->add_retained_resource<framebuffer_description, ::ospray::cpp::FrameBuffer>(
    "Right Eye Framebuffer", framebuffer_description { size, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM });

  auto render_task = renderer->add_render_task<create_hmd_textures_task_data>(
    "Create HMD Render Pass",
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