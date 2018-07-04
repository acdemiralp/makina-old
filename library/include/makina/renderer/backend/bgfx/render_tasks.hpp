#ifndef MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_HPP
#define MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_HPP

#include <makina/renderer/backend/bgfx/render_tasks/present_render_task.hpp>
#include <makina/renderer/backend/bgfx/render_tasks/test_render_task.hpp>
#include <makina/export.hpp>

namespace di
{
class window;
}

namespace mak
{
class engine;
  
namespace bgfx
{
MAKINA_EXPORT void make_default_framegraph(engine* engine, di::window* main_window);
}
}

#endif