#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_OPENGL_ADAPTER_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_OPENGL_ADAPTER_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/framebuffer.hpp>
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
struct MAKINA_EXPORT opengl_adapter_task_data
{
  ospray::framebuffer_resource* source;
  opengl::framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<opengl_adapter_task_data>* add_opengl_adapter_render_task(
  renderer*                     renderer, 
  ospray::framebuffer_resource* source  , 
  opengl::framebuffer_resource* target  = nullptr);
}
}

#endif