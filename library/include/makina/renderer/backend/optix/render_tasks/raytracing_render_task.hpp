#ifndef MAKINA_RENDERER_BACKEND_OPTIX_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPTIX_RENDER_TASKS_RAYTRACING_RENDER_TASK_HPP_

#include <string>

#include <makina/renderer/backend/optix/render_tasks/upload_scene_render_task.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace optix
{
struct MAKINA_EXPORT raytracing_task_data
{

};

MAKINA_EXPORT fg::render_task<raytracing_task_data>* add_raytracing_render_task(renderer* framegraph, const upload_scene_task_data& scene_data, std::string camera_tag = std::string());
}
}

#endif