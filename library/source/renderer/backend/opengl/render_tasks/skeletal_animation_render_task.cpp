#include <makina/renderer/backend/opengl/render_tasks/skeletal_animation_render_task.hpp>

#include <gl/command_execution.hpp>
#include <gl/compute.hpp>
#include <gl/draw_commands.hpp>

#include <makina/renderer/backend/glsl/skeletal_animation_compute_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<skeletal_animation_task_data>* add_skeletal_animation_render_task(renderer* framegraph, const upload_scene_task_data& scene_data)
{
  const auto retained_vertices = framegraph->add_retained_resource<buffer_description, gl::buffer>(
    "Skeletal Animation Vertices", buffer_description{GLsizeiptr(64e+6), GL_SHADER_STORAGE_BUFFER});
  const auto retained_normals  = framegraph->add_retained_resource<buffer_description, gl::buffer>(
    "Skeletal Animation Normals" , buffer_description{GLsizeiptr(64e+6), GL_SHADER_STORAGE_BUFFER});
  // Totals to 64 * 2 = 128 MB of GPU memory for buffers.

  const auto retained_program = framegraph->add_retained_resource<compute_program_resource::description_type, program>("Skeletal Animation Program", program::compute_description
  {
    glsl::skeletal_animation_compute_shader
  });

  return framegraph->add_render_task<skeletal_animation_task_data>(
    "Skeletal Animation Pass",
    [&] (      skeletal_animation_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices             = builder.read(scene_data.vertices     );
      data.normals              = builder.read(scene_data.normals      );
      data.bone_ids             = builder.read(scene_data.bone_ids     );
      data.bone_weights         = builder.read(scene_data.bone_weights );
      data.rigs                 = builder.read(scene_data.rigs         );
      data.parameter_map        = builder.read(scene_data.parameter_map);
      data.program              = builder.read(retained_program);
      data.transformed_vertices = builder.write(retained_vertices);
      data.transformed_normals  = builder.write(retained_normals );
      data.vertex_array         = builder.create<vertex_array_resource>("Skeletal Animation Vertex Array", vertex_array::description
      {
        { }, 
        {
          data.transformed_vertices,
          data.transformed_normals ,
          data.bone_ids            ,
          data.bone_weights        ,
          data.rigs                ,
        }
      });
    },
    [=] (const skeletal_animation_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      
      auto vertex_count = data.parameter_map->actual()->get<GLuint>("vertex_count");

      auto vertices     = data.vertices->actual()->sub_data(0, vertex_count * sizeof glm::vec3);
      auto normals      = data.normals ->actual()->sub_data(0, vertex_count * sizeof glm::vec3);
      auto vertices_ptr = reinterpret_cast<glm::vec3*>(vertices.data());
      auto normals_ptr  = reinterpret_cast<glm::vec3*>(normals .data());
      std::vector<glm::vec4> input_vertices(vertex_count);
      std::vector<glm::vec4> input_normals (vertex_count);
      for (auto i = 0; i < vertex_count; ++i)
      {
        input_vertices[i] = glm::vec4(vertices_ptr[i], 1.0f);
        input_normals [i] = glm::vec4(normals_ptr [i], 0.0f);
      }
      data.transformed_vertices->actual()->set_sub_data(0, vertex_count * sizeof glm::vec4, input_vertices.data());
      data.transformed_normals ->actual()->set_sub_data(0, vertex_count * sizeof glm::vec4, input_normals .data());

      glDispatchComputeGroupSizeARB(vertex_count, 1, 1, 1, 1, 1);
      gl::memory_barrier(GL_SHADER_STORAGE_BARRIER_BIT);
      
      auto transformed_vertices     = data.transformed_vertices->actual()->sub_data(0, vertex_count * sizeof glm::vec4);
      auto transformed_normals      = data.transformed_normals ->actual()->sub_data(0, vertex_count * sizeof glm::vec4);
      auto transformed_vertices_ptr = reinterpret_cast<glm::vec4*>(transformed_vertices.data());
      auto transformed_normals_ptr  = reinterpret_cast<glm::vec4*>(transformed_normals .data());
      std::vector<glm::vec3> output_vertices(vertex_count);
      std::vector<glm::vec3> output_normals (vertex_count);
      for (auto i = 0; i < vertex_count; ++i)
      {
        output_vertices[i] = glm::vec3(transformed_vertices_ptr[i]);
        output_normals [i] = glm::vec3(transformed_normals_ptr [i]);
      }
      data.transformed_vertices->actual()->set_sub_data(0, vertex_count * sizeof glm::vec3, output_vertices.data());
      data.transformed_normals ->actual()->set_sub_data(0, vertex_count * sizeof glm::vec3, output_normals .data());

      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Skeletal Animation Pass");
    });
}
}
}
