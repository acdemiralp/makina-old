#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_IMMEDIATE_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_IMMEDIATE_RENDER_TASK_HPP_

#include <functional>
#include <string>

#include <im3d.h>

#include <makina/renderer/backend/opengl/render_tasks/upload_scene_render_task.hpp>
#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT immediate_task_data
{
  buffer_resource*                           attributes       ;
  buffer_resource*                           cameras          ;
  buffer_resource*                           viewport         ;
                                             
  graphics_program_resource*                 points_program   ;
  graphics_program_resource*                 lines_program    ;
  graphics_program_resource*                 triangles_program;
  vertex_array_resource*                     vertex_array     ;
  framebuffer_resource*                      target           ;
  std::function<void(const Im3d::DrawList&)> draw_callback    ;
};

MAKINA_EXPORT fg::render_task<immediate_task_data>* add_immediate_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data, const std::string& camera_tag = std::string());
}
}

#endif