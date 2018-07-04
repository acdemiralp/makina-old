#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_UI_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_UI_RENDER_TASK_HPP
                                                    
#include <makina/renderer/backend/vulkan/buffer.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/backend/vulkan/pipeline.hpp>
#include <makina/renderer/backend/vulkan/sampler.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT ui_task_data
{
  buffer_resource*      intermediates;

  buffer_resource*      vertices     ;
  buffer_resource*      indices      ;
  buffer_resource*      projection   ;
  image_resource*       image        ;
  sampler_resource*     sampler      ;
                                     
  pipeline_resource*    pipeline     ;
  framebuffer_resource* target       ;
};

MAKINA_EXPORT fg::render_task<ui_task_data>* add_ui_render_task(renderer* framegraph, framebuffer_resource* target);
}
}

#endif