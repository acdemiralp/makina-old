#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_VOLUMES_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_UPLOAD_VOLUMES_RENDER_TASK_HPP_

#include <map>
#include <memory>

#include <fg/render_task.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/renderer/renderer.hpp>
#include <makina/renderer/volume_render.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT upload_volumes_task_data
{
  struct MAKINA_EXPORT volume_data
  {
    std::shared_ptr<::ospray::cpp::Volume>           volume           ;
    std::shared_ptr<::ospray::cpp::Data>             voxel_data       ;
    std::shared_ptr<::ospray::cpp::TransferFunction> transfer_function;
    std::shared_ptr<::ospray::cpp::Data>             colors           ;
    std::shared_ptr<::ospray::cpp::Data>             opacities        ;
  };

  std::map<volume_render*, volume_data> cache;
};

MAKINA_EXPORT fg::render_task<upload_volumes_task_data>* add_upload_volumes_render_task(renderer* renderer, bool only_raytracing = true);
}
}

#endif