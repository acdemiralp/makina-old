#include <makina/renderer/backend/vulkan/render_tasks.hpp>

#include <makina/renderer/backend/vulkan/vulkan_context.hpp>

namespace mak
{
fg::render_task<test_task_data>*    add_test_render_task   (renderer* framegraph)
{
  return framegraph->add_render_task<test_task_data>(
    "Test Pass",
    [&] (      test_task_data& data, fg::render_task_builder& builder)
    {
      
    },
    [=] (const test_task_data& data)
    {

    });
}
fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph)
{
  return framegraph->add_render_task<present_task_data>(
    "Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const present_task_data& data)
    {
      vulkan_context::get().present_window_swapchains();
    });
}
}
