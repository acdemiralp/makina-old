#ifndef MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_VULKAN_RENDER_TASKS_HPP_

#include <makina/renderer/backend/vulkan/render_tasks/phong_shading_render_task.hpp>
#include <makina/renderer/backend/vulkan/render_tasks/physically_based_shading_render_task.hpp>
#include <makina/renderer/backend/vulkan/render_tasks/present_render_task.hpp>
#include <makina/renderer/backend/vulkan/render_tasks/test_render_task.hpp>
#include <makina/renderer/backend/vulkan/render_tasks/ui_render_task.hpp>
#include <makina/renderer/backend/vulkan/render_tasks/upload_scene_render_task.hpp>
#include <makina/export.hpp>

namespace di
{
class vulkan_window;
}

namespace mak
{
class engine;

namespace vulkan
{
MAKINA_EXPORT void make_default_framegraph(engine* engine, di::vulkan_window* window);
}
}
#endif