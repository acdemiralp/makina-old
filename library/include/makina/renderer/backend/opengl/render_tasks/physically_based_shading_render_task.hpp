#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_PHYSICALLY_BASED_SHADING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_PHYSICALLY_BASED_SHADING_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/render_tasks/upload_scene_render_task.hpp>
#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/texture.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT physically_based_shading_task_data
{
  buffer_resource*                  vertices           ;
  buffer_resource*                  normals            ;
  buffer_resource*                  texture_coordinates;
  buffer_resource*                  instance_attributes;
  buffer_resource*                  indices            ;
  buffer_resource*                  transforms         ;
  buffer_resource*                  materials          ;
  buffer_resource*                  cameras            ;
  buffer_resource*                  lights             ;
  buffer_resource*                  draw_calls         ;
  parameter_map_resource*           parameter_map      ;
  std::vector<texture_2d_resource*> textures           ;

  program_resource*                 program            ;
  vertex_array_resource*            vertex_array       ;
  framebuffer_resource*             target             ;
};

MAKINA_EXPORT fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
}
}

#endif