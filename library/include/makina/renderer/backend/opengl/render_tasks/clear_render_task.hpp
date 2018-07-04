#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_CLEAR_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_CLEAR_RENDER_TASK_HPP

#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT clear_task_data
{
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<clear_task_data>* add_clear_render_task(renderer* framegraph, framebuffer_resource* target, const glm::vec4& color, const float depth = 1.0f);
}
}

#endif