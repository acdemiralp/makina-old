#include <makina/renderer/backend/ospray/render_tasks/opengl_adapter_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<opengl_adapter_task_data>* add_opengl_adapter_render_task(
  renderer*                     renderer,
  ospray::framebuffer_resource* source  , 
  opengl::framebuffer_resource* target  )
{
  if (!target)
    target = renderer->add_retained_resource<opengl::framebuffer::description, opengl::framebuffer>(
      "OpenGL Adapter Output Framebuffer", 
      opengl::framebuffer::description {{source->description().size[0], source->description().size[1]}});

  const auto render_task = renderer->add_render_task<opengl_adapter_task_data>(
    "Create HMD Render Pass",
    [&] (      opengl_adapter_task_data& data, fg::render_task_builder& builder)
    {
      data.source = builder.read (source);
      data.target = builder.write(target);
    },
    [=] (const opengl_adapter_task_data& data)
    {
      const auto colors = static_cast<std::uint8_t*>(data.source->actual()->map(OSP_FB_COLOR));
      const auto depths = static_cast<std::uint8_t*>(data.source->actual()->map(OSP_FB_DEPTH));
      data.target->actual()->color_texture()->set_sub_image(
        0, 
        0, 
        0, 
        data.source->description().size[0], 
        data.source->description().size[1], 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        colors);
      data.target->actual()->depth_texture()->set_sub_image(
        0,
        0,
        0,
        data.source->description().size[0],
        data.source->description().size[1],
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        depths);
      data.source->actual()->unmap(colors);
      data.source->actual()->unmap(depths);
    });
  return render_task;
}
}
}