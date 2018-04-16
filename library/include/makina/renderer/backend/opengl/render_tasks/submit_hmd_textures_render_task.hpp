#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_SUBMIT_HMD_TEXTURES_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_SUBMIT_HMD_TEXTURES_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/render_tasks/create_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace di
{
class hmd;
}

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT submit_hmd_textures_task_data
{
  framebuffer_resource* left ;
  framebuffer_resource* right;
};

MAKINA_EXPORT fg::render_task<submit_hmd_textures_task_data>* add_submit_hmd_textures_render_task(renderer* framegraph, di::hmd* hmd, const create_hmd_textures_task_data& texture_data);
}
}

#endif