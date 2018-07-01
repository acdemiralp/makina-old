#include <makina/renderer/backend/ospray/render_tasks/raytracing_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<raytracing_task_data>* add_raytracing_render_task(
  renderer*                 renderer   ,
  upload_common_task_data*  common_data, 
  upload_points_task_data*  point_data , 
  upload_lines_task_data*   line_data  ,
  upload_meshes_task_data*  mesh_data  , 
  upload_volumes_task_data* volume_data, 
  framebuffer_resource*     target     ,
  const std::string&        camera_tag )
{
  return renderer->add_render_task<raytracing_task_data>(
    "Raytracing Pass",
    [=] (      raytracing_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [&] (const raytracing_task_data& data)
    {
      
    });
}
}
}