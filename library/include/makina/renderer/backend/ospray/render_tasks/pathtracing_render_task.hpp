#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_PATHTRACING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_PATHTRACING_RENDER_TASK_HPP_

#include <fg/render_task.hpp>

#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT pathtracing_task_data
{
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<pathtracing_task_data>* add_pathtracing_render_task(renderer* renderer, framebuffer_resource* target, const std::string& camera_tag = std::string());
}
}

#endif