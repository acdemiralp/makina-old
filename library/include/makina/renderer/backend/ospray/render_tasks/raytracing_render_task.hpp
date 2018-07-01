#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_

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
struct MAKINA_EXPORT raytracing_settings
{
  bool          one_sided_lighting        = false;
  bool          shadows_enabled           = true ;
  std::uint32_t ambient_occlusion_samples = 1;
  std::uint32_t samples_per_pixel         = 1;
  glm::vec4     background_color          = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f);
  
};
struct MAKINA_EXPORT raytracing_task_data
{
  framebuffer_resource*                    target          ;
  std::unique_ptr<::ospray::cpp::Renderer> renderer        ;
  std::unique_ptr<::ospray::cpp::Model>    model           ;
  std::set<::ospray::cpp::Geometry*>       geometry_cache  ; 
  std::set<::ospray::cpp::Volume*>         volume_cache    ; 
  std::optional<transform>                 transform_cache ;
  std::optional<projection>                projection_cache;
};

MAKINA_EXPORT fg::render_task<raytracing_task_data>* add_raytracing_render_task(
  renderer*                       renderer   , 
  const upload_common_task_data*  common_data,
  const upload_points_task_data*  point_data ,
  const upload_lines_task_data*   line_data  ,
  const upload_meshes_task_data*  mesh_data  ,
  const upload_volumes_task_data* volume_data,
  framebuffer_resource*           target     , 
  const std::string&              camera_tag = std::string        (),
  const raytracing_settings&      settings   = raytracing_settings());
}
}

#endif