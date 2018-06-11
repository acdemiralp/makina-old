#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP_

#include <fg/render_task.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT upload_common_task_data
{
  upload_common_task_data           ()                                     = default;
  upload_common_task_data           (const upload_common_task_data&  that) = delete ;
  upload_common_task_data           (      upload_common_task_data&& temp) = default;
  virtual ~upload_common_task_data  ()                                     = default;
  upload_common_task_data& operator=(const upload_common_task_data&  that) = delete ;
  upload_common_task_data& operator=(      upload_common_task_data&& temp) = default;

  std::vector<::ospray::cpp::Camera> cameras;
  std::vector<::ospray::cpp::Data>   lights ;
};

MAKINA_EXPORT fg::render_task<upload_common_task_data>* add_upload_common_render_task(renderer* renderer, bool only_raytracing = true);
}
}

#endif