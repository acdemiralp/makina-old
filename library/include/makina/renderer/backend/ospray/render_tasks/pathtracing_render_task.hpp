#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_PATHTRACING_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_PATHTRACING_RENDER_TASK_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <set>

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
struct MAKINA_EXPORT pathtracing_settings
{
  std::uint32_t roulette_depth    = 5;
  std::uint32_t samples_per_pixel = 1;
};
struct MAKINA_EXPORT pathtracing_task_data
{
  framebuffer_resource*                    target          ;
  std::unique_ptr<::ospray::cpp::Renderer> renderer        ;
  std::unique_ptr<::ospray::cpp::Model>    model           ;
  std::set<::ospray::cpp::Geometry*>       geometry_cache  ; 
  std::set<::ospray::cpp::Volume*>         volume_cache    ; 
  std::optional<transform>                 transform_cache ;
  std::optional<projection>                projection_cache;
};

MAKINA_EXPORT fg::render_task<pathtracing_task_data>* add_pathtracing_render_task(
  renderer*                       renderer   , 
  const upload_common_task_data*  common_data,
  const upload_points_task_data*  point_data ,
  const upload_lines_task_data*   line_data  ,
  const upload_meshes_task_data*  mesh_data  ,
  const upload_volumes_task_data* volume_data,
  framebuffer_resource*           target     , 
  const std::string&              camera_tag = std::string         (),
  const pathtracing_settings&     settings   = pathtracing_settings());
}
}

#endif