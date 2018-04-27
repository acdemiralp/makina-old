#include <makina/renderer/backend/opengl/render_tasks/test_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/draw_commands.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/test_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/test_vertex_shader.hpp>

namespace mak
{
namespace opengl
{
 fg::render_task<test_task_data>* add_test_render_task (renderer* framegraph, framebuffer_resource* target)
{
  return framegraph->add_render_task<test_task_data>(
    "Test Shading Pass",
    [&] (      test_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices     = builder.create<buffer_resource>          ("Test Shading Vertices"    , buffer_description           {GLsizeiptr(1e+6), GL_ARRAY_BUFFER});
      data.program      = builder.create<graphics_program_resource>("Test Shading Program"     , program::graphics_description{glsl::test_vertex_shader, glsl::test_fragment_shader});
      data.vertex_array = builder.create<vertex_array_resource>    ("Test Shading Vertex Array", vertex_array::description    {{{data.vertices, 3, GL_FLOAT}}});
      data.target       = builder.write(target);
    },
    [=] (const test_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();
      
      std::vector<glm::vec3> points = {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f}};
      data.vertices->actual()->set_sub_data(0, sizeof(glm::vec3) * points.size(), points.data());
      
      gl::set_viewport({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      gl::draw_arrays (GL_TRIANGLE_STRIP, 0, 5);

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Test Shading Pass");
    });
}
   
}
}