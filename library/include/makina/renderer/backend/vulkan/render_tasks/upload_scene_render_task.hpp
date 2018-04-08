#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_UPLOAD_SCENE_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_UPLOAD_SCENE_RENDER_TASK_HPP_

#include <makina/renderer/backend/vulkan/buffer.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/backend/vulkan/sampler.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT upload_scene_task_data
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
};

MAKINA_EXPORT fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph);
}
}

#endif