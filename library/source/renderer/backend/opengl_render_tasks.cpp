#include <makina/renderer/backend/opengl_render_tasks.hpp>

#include <gl/all.hpp>

namespace mak
{
fg::render_task<clear_task_data>* add_clear_render_task(fg::framegraph* framegraph, const glm::vec4& color)
{
  return framegraph->add_render_task<clear_task_data>(
    "Clear Pass",
    [&] (      clear_task_data& data, fg::render_task_builder& builder)
    {
      gl::set_clear_color({color[0], color[1], color[2], color[3]});
    },
    [=] (const clear_task_data& data)
    {
      gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
}

fg::render_task<phong_task_data>* add_phong_render_task(fg::framegraph* framegraph)
{
  return framegraph->add_render_task<phong_task_data>(
    "Phong Shading Pass",
    [&] (      phong_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const phong_task_data& data)
    {

    });
}
}
