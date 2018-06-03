#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_COMPOSITE_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_COMPOSITE_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT composite_task_data
{
  framebuffer_resource*      source      ;
  framebuffer_resource*      target      ;

  buffer_resource*           vertices    ;
  buffer_resource*           indices     ;
  graphics_program_resource* program     ;
  vertex_array_resource*     vertex_array;
};

MAKINA_EXPORT fg::render_task<composite_task_data>* add_composite_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target);
}
}

#endif