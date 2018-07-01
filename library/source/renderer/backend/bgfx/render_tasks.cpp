#include <makina/renderer/backend/bgfx/render_tasks.hpp>

#include <makina/core/engine.hpp>
#include <makina/renderer/backend/bgfx/context.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/ui/ui_system.hpp>

namespace mak
{
void bgfx::make_default_framegraph(engine* engine, di::window* main_window)
{
  engine->remove_system<ui_system>(); // BGFX has its native UI.

  context::initialize(::bgfx::RendererType::OpenGL, main_window);
  
  const auto renderer            = engine->get_system<mak::renderer>();
  auto       test_render_task    = mak::bgfx::add_test_render_task   (renderer); 
  auto       present_render_task = mak::bgfx::add_present_render_task(renderer);
}
}
