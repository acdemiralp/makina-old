#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_TEST_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_TEST_RENDER_TASK_HPP

#include <makina/renderer/backend/vulkan/buffer.hpp>
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

MAKINA_EXPORT fg::render_task<test_task_data>* add_test_render_task(renderer* framegraph);
}
}

#endif