#include <makina/renderer/backend/optix/render_tasks/upload_scene_render_task.hpp>

namespace mak
{
namespace optix
{
fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph)
{
  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [=] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      
    },
    [=] (const upload_scene_task_data& data)
    {

    });
}
}
}