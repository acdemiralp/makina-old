#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_LINES_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_LINES_RENDER_TASK_HPP

#include <map>
#include <memory>

#include <fg/render_task.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/renderer/line_render.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT upload_lines_task_data
{
  struct MAKINA_EXPORT line_data
  {
    std::shared_ptr<::ospray::cpp::Geometry> geometry;
    std::shared_ptr<::ospray::cpp::Data>     vertices;
    std::shared_ptr<::ospray::cpp::Data>     colors  ;
    std::shared_ptr<::ospray::cpp::Data>     indices ;
  };

  std::map<line_render*, line_data> cache;
};

MAKINA_EXPORT fg::render_task<upload_lines_task_data>* add_upload_lines_render_task(renderer* renderer, bool only_raytracing = true);
}
}

#endif