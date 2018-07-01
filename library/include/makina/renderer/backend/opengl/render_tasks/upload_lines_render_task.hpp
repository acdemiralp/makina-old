#ifndef MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_LINES_RENDER_TASK_HPP_
#define MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_LINES_RENDER_TASK_HPP_

#include <fg/render_task.hpp>
#include <gl/draw_commands.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT upload_lines_task_data
{
  buffer_resource*                                             vertices           ;
  buffer_resource*                                             colors             ;
  buffer_resource*                                             instance_attributes;
  buffer_resource*                                             indices            ;
  buffer_resource*                                             draw_calls         ;
  buffer_resource*                                             transforms         ;
  buffer_resource*                                             materials          ;
  parameter_map_resource*                                      parameter_map      ;
                                                               
  GLuint                                                       vertex_offset      = 0;
  GLuint                                                       index_offset       = 0;

  std::map<line_segments*, gl::draw_elements_indirect_command> line_segments_cache;
};

MAKINA_EXPORT fg::render_task<upload_lines_task_data>* add_upload_lines_render_task(renderer* framegraph);
}
}

#endif