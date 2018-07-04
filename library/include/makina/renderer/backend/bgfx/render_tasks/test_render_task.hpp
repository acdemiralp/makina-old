#ifndef MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_TEST_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_BGFX_RENDER_TASKS_TEST_RENDER_TASK_HPP

#include <fg/render_task.hpp>

#include <makina/renderer/backend/bgfx/buffer.hpp>
#include <makina/renderer/backend/bgfx/program.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace bgfx
{
struct MAKINA_EXPORT test_task_data
{
  buffer_resource*  vertices;
  buffer_resource*  indices;
  program_resource* program;
};

MAKINA_EXPORT fg::render_task<test_task_data>* add_test_render_task(renderer* framegraph);
}
}

#endif