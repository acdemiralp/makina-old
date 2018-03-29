#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_

#include <fg/render_task.hpp>

#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT clear_task_data
{
  image_resource* target;
};
struct MAKINA_EXPORT present_task_data
{
  
};

MAKINA_EXPORT fg::render_task<clear_task_data>*   add_clear_render_task  (renderer* framegraph, image_resource* target, const glm::vec4& color, const float depth = 1.0f);
MAKINA_EXPORT fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph);
}

#endif