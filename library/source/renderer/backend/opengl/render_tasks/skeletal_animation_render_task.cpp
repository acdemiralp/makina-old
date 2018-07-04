#include <makina/renderer/backend/opengl/render_tasks/skeletal_animation_render_task.hpp>

#include <gl/command_execution.hpp>
#include <gl/draw_commands.hpp>

#include <makina/renderer/backend/glsl/skeletal_animation_compute_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<skeletal_animation_task_data>* add_skeletal_animation_render_task(renderer* framegraph, const upload_meshes_task_data& mesh_data)
{
  const auto retained_vertices = framegraph->add_retained_resource<buffer_description                        , gl::buffer>("Skeletal Animation Vertices", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_normals  = framegraph->add_retained_resource<buffer_description                        , gl::buffer>("Skeletal Animation Normals" , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_program  = framegraph->add_retained_resource<compute_program_resource::description_type, program>   ("Skeletal Animation Program" , program::compute_description
  {
    glsl::skeletal_animation_compute_shader
  });

  return framegraph->add_render_task<skeletal_animation_task_data>(
    "Skeletal Animation Pass",
    [&] (      skeletal_animation_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices             = builder.read (mesh_data.vertices     );
      data.normals              = builder.read (mesh_data.normals      );
      data.bone_ids             = builder.read (mesh_data.bone_ids     );
      data.bone_weights         = builder.read (mesh_data.bone_weights );
      data.rigs                 = builder.read (mesh_data.rigs         );
      data.parameter_map        = builder.read (mesh_data.parameter_map);
      data.program              = builder.read (retained_program       );
      data.transformed_vertices = builder.write(retained_vertices      );
      data.transformed_normals  = builder.write(retained_normals       );
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
      data.transformed_vertices->actual()->copy_sub_data(*data.vertices->actual(), 0, 0, vertex_count * sizeof glm::vec4);
      data.transformed_normals ->actual()->copy_sub_data(*data.normals ->actual(), 0, 0, vertex_count * sizeof glm::vec4);
      
      glDispatchComputeGroupSizeARB(vertex_count, 1, 1, 1, 1, 1);
      gl::memory_barrier(GL_SHADER_STORAGE_BARRIER_BIT);
      
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Skeletal Animation Pass");
    });
}
}
}
