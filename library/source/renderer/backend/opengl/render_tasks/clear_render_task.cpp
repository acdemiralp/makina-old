#include <makina/renderer/backend/opengl/render_tasks/clear_render_task.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<clear_task_data>* add_clear_render_task(renderer* framegraph, framebuffer_resource* target, const glm::vec4& color, const float depth)
{
  return framegraph->add_render_task<clear_task_data>(
    "Clear Pass",
    [&] (      clear_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [=] (const clear_task_data& data)
    {
      data.target->actual()->clear_color(std::array<float, 4>{color[0], color[1], color[2], color[3]});
      data.target->actual()->clear_depth_and_stencil(depth, 0);

      gl::print_error("Error in Clear Pass");
    });
}
}
}