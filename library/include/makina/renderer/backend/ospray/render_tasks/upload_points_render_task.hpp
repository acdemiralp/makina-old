#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_POINTS_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_POINTS_RENDER_TASK_HPP_

#include <map>
#include <memory>

#include <fg/render_task.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/renderer/point_render.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT upload_points_task_data
{
  struct MAKINA_EXPORT point_data
  {
    std::shared_ptr<::ospray::cpp::Geometry> geometry;
    std::shared_ptr<::ospray::cpp::Data>     vertices;
    std::shared_ptr<::ospray::cpp::Data>     colors  ;
  };

  std::map<point_render*, point_data> cache;
};

MAKINA_EXPORT fg::render_task<upload_points_task_data>* add_upload_points_render_task(renderer* renderer, bool only_raytracing = true);
}
}

#endif