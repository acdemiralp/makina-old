#include <makina/renderer/backend/opengl/render_tasks/skeletal_animation_render_task.hpp>

namespace mak
{
fg::render_task<opengl::skeletal_animation_task_data>* opengl::add_skeletal_animation_render_task(renderer* framegraph)
{
  return framegraph->add_render_task<skeletal_animation_task_data>(
    "Skeletal Animation Pass",
    [&] (      skeletal_animation_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const skeletal_animation_task_data& data)
    {

    });
}
}
