#ifndef MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP
#define MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_COMMON_RENDER_TASK_HPP

#include <fg/render_task.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT upload_common_task_data
{
  buffer_resource*        cameras      ;
  buffer_resource*        lights       ;
  parameter_map_resource* parameter_map;
};

MAKINA_EXPORT fg::render_task<upload_common_task_data>* add_upload_common_render_task(renderer* renderer);
}
}

#endif