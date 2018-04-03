#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_

#include <makina/renderer/backend/vulkan/buffer.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/backend/vulkan/pipeline.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT test_task_data
{
  buffer_resource*             vertices     ;

  pipeline_resource*           pipeline     ;
};
struct MAKINA_EXPORT upload_scene_task_data
{
  buffer_resource*             vertices     ;
  buffer_resource*             indices      ;
  buffer_resource*             transforms   ;
  buffer_resource*             materials    ;
  buffer_resource*             cameras      ;
  buffer_resource*             lights       ;
  buffer_resource*             draw_calls   ;
  parameter_map_resource*      parameter_map;
  std::vector<image_resource*> images       ;
};
struct MAKINA_EXPORT phong_task_data
{
  buffer_resource*             vertices     ;
  buffer_resource*             indices      ;
  buffer_resource*             transforms   ;
  buffer_resource*             materials    ;
  buffer_resource*             cameras      ;
  buffer_resource*             lights       ;
  buffer_resource*             draw_calls   ;
  parameter_map_resource*      parameter_map;
  std::vector<image_resource*> images       ;
  
  pipeline_resource*           pipeline     ;
  framebuffer_resource*        target       ;
};
struct MAKINA_EXPORT physically_based_shading_task_data
{
  buffer_resource*             vertices     ;
  buffer_resource*             indices      ;
  buffer_resource*             transforms   ;
  buffer_resource*             materials    ;
  buffer_resource*             cameras      ;
  buffer_resource*             lights       ;
  buffer_resource*             draw_calls   ;
  parameter_map_resource*      parameter_map;
  std::vector<image_resource*> images       ;
  
  pipeline_resource*           pipeline     ;
  framebuffer_resource*        target       ;
}; 
struct MAKINA_EXPORT ui_task_data
{
  buffer_resource*             vertices     ;
  buffer_resource*             indices      ;
  image_resource*              texture      ;

  pipeline_resource*           pipeline     ;
  framebuffer_resource*        target       ;
};
struct MAKINA_EXPORT present_task_data
{

};

MAKINA_EXPORT fg::render_task<test_task_data>*                     add_test_render_task                    (renderer* framegraph);
MAKINA_EXPORT fg::render_task<upload_scene_task_data>*             add_upload_scene_render_task            (renderer* framegraph);
MAKINA_EXPORT fg::render_task<phong_task_data>*                    add_phong_render_task                   (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
MAKINA_EXPORT fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
MAKINA_EXPORT fg::render_task<ui_task_data>*                       add_ui_render_task                      (renderer* framegraph, framebuffer_resource* target);
MAKINA_EXPORT fg::render_task<present_task_data>*                  add_present_render_task                 (renderer* framegraph);
}
}

#endif