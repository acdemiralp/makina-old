#ifndef MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_MESHES_RENDER_TASK_HPP_
#define MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_MESHES_RENDER_TASK_HPP_

#include <fg/render_task.hpp>

#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT upload_meshes_task_data
{
  
};

MAKINA_EXPORT fg::render_task<upload_meshes_task_data>* add_upload_meshes_render_task(renderer* renderer);
}

#endif