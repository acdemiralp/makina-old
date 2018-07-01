#include <makina/renderer/backend/ospray/render_tasks/pathtracing_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<pathtracing_task_data>* add_pathtracing_render_task(
  renderer*                       renderer   ,
  const upload_common_task_data&  common_data, 
  const upload_points_task_data&  point_data , 
  const upload_lines_task_data&   line_data  ,
  const upload_meshes_task_data&  mesh_data  , 
  const upload_volumes_task_data& volume_data, 
  framebuffer_resource*           target     ,
  const std::string&              camera_tag )
{
  return renderer->add_render_task<pathtracing_task_data>(
    "Pathtracing Pass",
    [=] (      pathtracing_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [&] (const pathtracing_task_data& data)
    {
      
    });
}
}
}