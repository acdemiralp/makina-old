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
    "Skeletal Animation Vertices", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER});
  const auto retained_normals  = framegraph->add_retained_resource<buffer_description, gl::buffer>(
    "Skeletal Animation Normals" , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER});
  // Totals to 64 * 2 = 128 MB of GPU memory for buffers.

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
      
      data.transformed_vertices = builder.write(retained_vertices);
      data.transformed_normals  = builder.write(retained_normals );
      
      data.program              = builder.create<compute_program_resource>("Skeletal Animation Program"     , program::compute_description     
      {
        glsl::skeletal_animation_compute_shader
      });
      data.vertex_array         = builder.create<vertex_array_resource>   ("Skeletal Animation Vertex Array", vertex_array::description
      {
        { }, 
        {
          data.vertices    ,
          data.normals     ,
          data.bone_ids    ,
          data.bone_weights,
          data.rigs  
        }
      });
    },
    [=] (const skeletal_animation_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      
      glDispatchComputeGroupSizeARB(1, 1, 1, 1, 1, 1);
      gl::memory_barrier(GL_SHADER_STORAGE_BARRIER_BIT);

      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Skeletal Animation Pass");
    });
}
}
}
