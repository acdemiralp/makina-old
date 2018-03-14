#include <makina/renderer/backend/opengl_render_tasks.hpp>

#include <gl/all.hpp>

namespace mak
{
fg::render_task<clear_task_data>* add_clear_render_task(fg::framegraph* framegraph, opengl_render_target* render_target, const glm::vec4& color)
{
  return framegraph->add_render_task<clear_task_data>(
    "Clear Pass",
    [&]     (      clear_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.read(render_target);
    },
    [color] (const clear_task_data& data)
    {
      data.target->clear_color(std::array<float, 4>{color[0], color[1], color[2], color[3]});
    });
}
fg::render_task<phong_task_data>* add_phong_render_task(fg::framegraph* framegraph, opengl_render_target* render_target)
{
  return framegraph->add_render_task<phong_task_data>(
    "Phong Shading Pass",
    [&] (      phong_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.read(render_target);
    },
    [=] (const phong_task_data& data)
    {
      // Take mesh, material, light, camera data and render onto data.target.
    });
}
}