#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_

#include <fg/framegraph.hpp>
#include <glm/vec4.hpp>

#include <makina/export.hpp>

namespace mak
{
struct clear_task_data { };
struct phong_task_data { };

MAKINA_EXPORT fg::render_task<clear_task_data>* add_clear_render_task(fg::framegraph* framegraph, const glm::vec4& color);
MAKINA_EXPORT fg::render_task<phong_task_data>* add_phong_render_task(fg::framegraph* framegraph);
}

#endif