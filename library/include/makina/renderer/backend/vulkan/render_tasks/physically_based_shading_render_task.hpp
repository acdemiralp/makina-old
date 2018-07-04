#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PHYSICALLY_BASED_SHADING_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PHYSICALLY_BASED_SHADING_RENDER_TASK_HPP

#include <makina/renderer/backend/vulkan/render_tasks/upload_scene_render_task.hpp>
#include <makina/renderer/backend/vulkan/buffer.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/backend/vulkan/pipeline.hpp>
#include <makina/renderer/backend/vulkan/sampler.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT physically_based_shading_task_data
{
  buffer_resource*        vertices     ;
  buffer_resource*        indices      ;
  buffer_resource*        transforms   ;
  buffer_resource*        materials    ;
  buffer_resource*        cameras      ;
  buffer_resource*        lights       ;
  buffer_resource*        draw_calls   ;
  image_resource*         images       ;
  sampler_resource*       sampler      ;
  parameter_map_resource* parameter_map;
                          
  pipeline_resource*      pipeline     ;
  framebuffer_resource*   target       ;
};

MAKINA_EXPORT fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data);
}
}

#endif