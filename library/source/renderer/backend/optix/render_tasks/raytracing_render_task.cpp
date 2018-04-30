#include <makina/renderer/backend/optix/render_tasks/raytracing_render_task.hpp>

namespace mak
{
namespace optix
{
fg::render_task<raytracing_task_data>* add_raytracing_render_task(renderer* framegraph, const upload_scene_task_data& scene_data, std::string camera_tag)
{
  return framegraph->add_render_task<raytracing_task_data>(
    "Raytracing Pass",
    [=] (      raytracing_task_data& data, fg::render_task_builder& builder)
    {
      
    },
    [=] (const raytracing_task_data& data)
    {

    });
}
}
}