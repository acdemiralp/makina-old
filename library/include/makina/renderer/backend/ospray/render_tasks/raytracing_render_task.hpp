#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_

#include <fg/render_task.hpp>

#include <makina/renderer/backend/ospray/render_tasks/upload_common_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_lines_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_meshes_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_points_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_volumes_render_task.hpp>
#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT raytracing_task_data
{
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<raytracing_task_data>* add_raytracing_render_task(
  renderer*                 renderer   , 
  upload_common_task_data*  common_data,
  upload_points_task_data*  point_data ,
  upload_lines_task_data*   line_data  ,
  upload_meshes_task_data*  mesh_data  ,
  upload_volumes_task_data* volume_data,
  framebuffer_resource*     target     , 
  const std::string&        camera_tag = std::string());
}
}

#endif