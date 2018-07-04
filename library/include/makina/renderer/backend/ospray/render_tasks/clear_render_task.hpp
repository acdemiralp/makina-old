#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_CLEAR_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_CLEAR_RENDER_TASK_HPP

#include <fg/render_task.hpp>

#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT clear_task_data
{
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<clear_task_data>* add_clear_render_task(renderer* renderer, framebuffer_resource* target);
}
}

#endif