#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_

#include <fg/framegraph.hpp>
#include <glm/vec4.hpp>

#include <makina/renderer/backend/opengl_resources.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT clear_task_data
{
  render_target_resource* target;
};
struct MAKINA_EXPORT phong_task_data
{
  render_target_resource* target;
};

MAKINA_EXPORT fg::render_task<clear_task_data>* add_clear_render_task(fg::framegraph* framegraph, render_target_resource* render_target, const glm::vec4& color);
MAKINA_EXPORT fg::render_task<phong_task_data>* add_phong_render_task(fg::framegraph* framegraph, render_target_resource* render_target);
}

#endif