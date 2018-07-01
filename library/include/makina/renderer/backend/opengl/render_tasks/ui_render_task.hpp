#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_UI_RENDER_TASK_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_UI_RENDER_TASK_HPP_

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/opengl/program.hpp>
#include <makina/renderer/backend/opengl/texture.hpp>
#include <makina/renderer/backend/opengl/vertex_array.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT ui_task_data
{
  buffer_resource*           attributes  ;
  buffer_resource*           indices     ;
  buffer_resource*           projection  ;
  texture_2d_resource*       texture     ;
                         
  graphics_program_resource* program     ;
  vertex_array_resource*     vertex_array;
  framebuffer_resource*      target      ;
};

MAKINA_EXPORT fg::render_task<ui_task_data>* add_ui_render_task(renderer* framegraph, framebuffer_resource* target);
}
}

#endif