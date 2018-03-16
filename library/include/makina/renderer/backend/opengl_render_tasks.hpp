#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_

#include <glm/vec4.hpp>

#include <makina/renderer/backend/opengl_resources.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT upload_scene_task_data
{
  buffer_resource*                  vertices           ;
  buffer_resource*                  normals            ;
  buffer_resource*                  texture_coordinates;
  buffer_resource*                  instance_attributes; // [0] transform id, [1] material id, [2] index offset
  buffer_resource*                  indices            ;
  buffer_resource*                  transforms         ;
  buffer_resource*                  materials          ;
  buffer_resource*                  cameras            ;
  buffer_resource*                  lights             ;
  std::vector<texture_2d_resource*> textures           ;
};
struct MAKINA_EXPORT clear_task_data
{
  render_target_resource*           target             ;
};
struct MAKINA_EXPORT phong_task_data
{
  render_target_resource*           target             ;
};

MAKINA_EXPORT fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph);
MAKINA_EXPORT fg::render_task<clear_task_data>*        add_clear_render_task       (renderer* framegraph, render_target_resource* render_target, const glm::vec4& color);
MAKINA_EXPORT fg::render_task<phong_task_data>*        add_phong_render_task       (renderer* framegraph, render_target_resource* render_target, const upload_scene_task_data& scene_data);
}

#endif