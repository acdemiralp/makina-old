#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_CREATE_HMD_TEXTURES_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_CREATE_HMD_TEXTURES_RENDER_TASK_HPP

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
struct MAKINA_EXPORT create_hmd_textures_task_data
{
  framebuffer_resource* left ;
  framebuffer_resource* right;
};

MAKINA_EXPORT fg::render_task<create_hmd_textures_task_data>* add_create_hmd_textures_render_task(renderer* framegraph, di::hmd* hmd);
}
}

#endif