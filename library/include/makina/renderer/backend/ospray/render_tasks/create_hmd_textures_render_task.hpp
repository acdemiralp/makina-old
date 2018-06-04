#ifndef MAKINA_RENDERER_OSPRAY_RENDER_TASKS_CREATE_HMD_TEXTURES_RENDER_TASK_HPP_
#define MAKINA_RENDERER_OSPRAY_RENDER_TASKS_CREATE_HMD_TEXTURES_RENDER_TASK_HPP_

#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace di
{
class hmd;
}

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT create_hmd_textures_task_data
{
  framebuffer_resource* left ;
  framebuffer_resource* right;
};

MAKINA_EXPORT fg::render_task<create_hmd_textures_task_data>* add_create_hmd_textures_render_task(renderer* renderer, di::hmd* hmd);
}
}

#endif