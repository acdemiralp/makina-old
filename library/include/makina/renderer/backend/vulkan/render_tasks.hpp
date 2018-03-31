#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_

#include <makina/renderer/backend/vulkan/buffer.hpp>
#include <makina/renderer/backend/vulkan/framebuffer.hpp>
#include <makina/renderer/backend/vulkan/image.hpp>
#include <makina/renderer/backend/vulkan/pipeline.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace vulkan
{
struct MAKINA_EXPORT test_task_data
{
  buffer_resource*   vertices;
  pipeline_resource* pipeline;
};
struct MAKINA_EXPORT present_task_data
{

};

MAKINA_EXPORT fg::render_task<test_task_data>*    add_test_render_task   (renderer* framegraph);
MAKINA_EXPORT fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph);
}
}

#endif