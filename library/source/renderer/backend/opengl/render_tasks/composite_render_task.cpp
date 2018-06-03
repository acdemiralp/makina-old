#include <makina/renderer/backend/opengl/render_tasks/composite_render_task.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<composite_task_data>* add_composite_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target)
{
  return framegraph->add_render_task<composite_task_data>(
    "Composite Pass",
    [&] (      composite_task_data& data, fg::render_task_builder& builder)
    {
      data.source = builder.read (source);
      data.target = builder.write(target);
    },
    [=] (const composite_task_data& data)
    {
      auto source = data.source->actual();
      auto target = data.target->actual();

      target->blit(
        *source, 
        0, 
        0, 
        source->color_texture()->width (),
        source->color_texture()->height(), 
        0, 
        0, 
        target->color_texture()->width (),
        target->color_texture()->height(), 
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

      gl::print_error("Error in Composite Pass");
    });
}
}
}