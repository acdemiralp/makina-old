#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_SKELETAL_ANIMATION_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_SKELETAL_ANIMATION_RENDER_TASK_HPP

#include <makina/renderer/backend/opengl/render_tasks/upload_meshes_render_task.hpp>
#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT skeletal_animation_task_data
{
  buffer_resource*          vertices            ;
  buffer_resource*          normals             ;
  buffer_resource*          bone_ids            ;
  buffer_resource*          bone_weights        ;
  buffer_resource*          rigs                ;
  parameter_map_resource*   parameter_map       ;
  
  buffer_resource*          transformed_vertices;
  buffer_resource*          transformed_normals ;

  compute_program_resource* program             ;
  vertex_array_resource*    vertex_array        ;
};

MAKINA_EXPORT fg::render_task<skeletal_animation_task_data>* add_skeletal_animation_render_task(renderer* framegraph, const upload_meshes_task_data& mesh_data);
}
}

#endif