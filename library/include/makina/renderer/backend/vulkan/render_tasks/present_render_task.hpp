#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PRESENT_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_PRESENT_RENDER_TASK_HPP

#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT present_task_data
{
  framebuffer_resource* backbuffer;
};

MAKINA_EXPORT fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph, framebuffer_resource* target);
}
}

#endif