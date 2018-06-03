#include <makina/renderer/backend/opengl/render_tasks/composite_render_task.hpp>

#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/composite_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/composite_fragment_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<composite_task_data>* add_composite_render_task(renderer* framegraph, framebuffer_resource* source, framebuffer_resource* target)
{
  const auto retained_vertices = framegraph->add_retained_resource<buffer_description, gl::buffer>("Composite Vertices", buffer_description{GLsizeiptr(1e+6)});
  const auto retained_indices  = framegraph->add_retained_resource<buffer_description, gl::buffer>("Composite Indices" , buffer_description{GLsizeiptr(1e+6)});
  const auto retained_program  = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Composite Program", program::graphics_description
  {
    glsl::composite_vertex_shader,
    glsl::composite_fragment_shader
  });
  
  return framegraph->add_render_task<composite_task_data>(
    "Composite Pass",
    [&] (      composite_task_data& data, fg::render_task_builder& builder)
    {
      data.source       = builder.read (source);
      data.target       = builder.write(target);
      data.vertices     = builder.read (retained_vertices);
      data.indices      = builder.read (retained_indices );
      data.program      = builder.read (retained_program );
      data.vertex_array = builder.create<vertex_array_resource>("Composite Vertex Array", vertex_array::description
      {
        {{data.vertices, 2, GL_FLOAT}}, 
        {}, 
        data.indices,
        nullptr
      });
    },
    [=] (const composite_task_data& data)
    {
      std::vector<glm::vec2> vertices = {{-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}};
      std::vector<glm::uint> indices  = {1, 3, 2, 1, 2, 0};
      data.vertices->actual()->set_sub_data(0, sizeof(glm::vec2) * vertices.size(), vertices.data());
      data.indices ->actual()->set_sub_data(0, sizeof(glm::uint) * indices .size(), indices .data());
    
      gl::texture_handle color_handle(*data.source->actual()->color_texture());
      gl::texture_handle depth_handle(*data.source->actual()->depth_texture());
      color_handle.set_resident(true);
      depth_handle.set_resident(true);
      data.program->actual()->set_uniform_handle(0, color_handle);
      data.program->actual()->set_uniform_handle(1, depth_handle);
      
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();
      
      glClipControl                 (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled    (true);
      gl::set_depth_function        (GL_LESS);
      gl::set_viewport              ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      gl::draw_elements             (GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT);

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      color_handle.set_resident(false);
      depth_handle.set_resident(false);

      gl::print_error("Error in Composite Pass");
    });
}
}
}