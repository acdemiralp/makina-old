#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_

#include <glm/vec4.hpp>

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
struct MAKINA_EXPORT test_task_data
{
  buffer_resource*                  vertices           ;
  program_resource*                 program            ;
  vertex_array_resource*            vertex_array       ;
  framebuffer_resource*             target             ;
};
struct MAKINA_EXPORT upload_scene_task_data
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
};
struct MAKINA_EXPORT clear_task_data
{
  framebuffer_resource*             target             ;
};
struct MAKINA_EXPORT phong_task_data
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
struct MAKINA_EXPORT ui_task_data
{
  buffer_resource*                  attributes         ;
  buffer_resource*                  indices            ;
  texture_2d_resource*              texture            ;

  program_resource*                 program            ;
  vertex_array_resource*            vertex_array       ;
  framebuffer_resource*             target             ;
};

MAKINA_EXPORT fg::render_task<test_task_data>*                     add_test_render_task                    (renderer* framegraph, framebuffer_resource* target);
MAKINA_EXPORT fg::render_task<upload_scene_task_data>*             add_upload_scene_render_task            (renderer* framegraph);
MAKINA_EXPORT fg::render_task<clear_task_data>*                    add_clear_render_task                   (renderer* framegraph, framebuffer_resource* target, const glm::vec4& color, const float depth = 1.0f);
MAKINA_EXPORT fg::render_task<phong_task_data>*                    add_phong_render_task                   (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
MAKINA_EXPORT fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
MAKINA_EXPORT fg::render_task<ui_task_data>*                       add_ui_render_task                      (renderer* framegraph, framebuffer_resource* target);
}

#endif