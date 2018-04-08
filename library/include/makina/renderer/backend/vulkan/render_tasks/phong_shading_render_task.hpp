#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PHONG_SHADING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PHONG_SHADING_RENDER_TASK_HPP_

#include <makina/renderer/backend/vulkan/render_tasks/upload_scene_render_task.hpp>
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
struct MAKINA_EXPORT phong_shading_task_data
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

MAKINA_EXPORT fg::render_task<phong_shading_task_data>* add_phong_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
}
}

#endif