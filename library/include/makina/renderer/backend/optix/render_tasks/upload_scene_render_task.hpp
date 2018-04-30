#ifndef MAKINA_RENDERER_BACKEND_OPTIX_RENDER_TASKS_UPLOAD_SCENE_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPTIX_RENDER_TASKS_UPLOAD_SCENE_RENDER_TASK_HPP_

#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace optix
{
struct MAKINA_EXPORT upload_scene_task_data
{

};

MAKINA_EXPORT fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph);
}
}

#endif