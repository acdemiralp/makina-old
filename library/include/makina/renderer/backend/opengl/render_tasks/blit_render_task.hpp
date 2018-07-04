#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_BLIT_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_BLIT_RENDER_TASK_HPP

#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT blit_task_data
{
  framebuffer_resource* source;
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<blit_task_data>* add_blit_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target);
}
}

#endif