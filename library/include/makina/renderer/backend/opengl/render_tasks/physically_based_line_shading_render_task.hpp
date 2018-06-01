#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_PHYSICALLY_BASED_LINE_SHADING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_PHYSICALLY_BASED_LINE_SHADING_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/render_tasks/upload_common_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/upload_lines_render_task.hpp>
#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT physically_based_line_shading_task_data
{
  buffer_resource*           vertices           ;
  buffer_resource*           colors             ;
  buffer_resource*           instance_attributes;
  buffer_resource*           indices            ;
  buffer_resource*           draw_calls         ;
  buffer_resource*           transforms         ;
  buffer_resource*           materials          ;
  buffer_resource*           cameras            ;
  buffer_resource*           lights             ;
  parameter_map_resource*    parameter_map      ;

  buffer_resource*           viewport           ;
  graphics_program_resource* program            ;
  vertex_array_resource*     vertex_array       ;
  framebuffer_resource*      target             ;
};

MAKINA_EXPORT fg::render_task<physically_based_line_shading_task_data>* add_physically_based_line_shading_render_task(
  renderer*                      framegraph , 
  framebuffer_resource*          target     , 
  const upload_common_task_data& common_data, 
  const upload_lines_task_data&  line_data  , 
  const std::string&             camera_tag = std::string());
}
}

#endif