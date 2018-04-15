#ifndef MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_PRESENT_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_PRESENT_RENDER_TASK_HPP_

#include <fg/render_task.hpp>

#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace bgfx
{
struct MAKINA_EXPORT present_task_data
{
  
};

MAKINA_EXPORT fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph);
}
}

#endif