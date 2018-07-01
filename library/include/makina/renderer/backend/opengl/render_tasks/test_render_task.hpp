#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_TEST_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_TEST_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT test_task_data
{
  buffer_resource*       vertices    ;
  graphics_program_resource*      program     ;
  vertex_array_resource* vertex_array;
  framebuffer_resource*  target      ;
};

MAKINA_EXPORT fg::render_task<test_task_data>* add_test_render_task(renderer* framegraph, framebuffer_resource* target);
}
}

#endif