#include <makina/renderer/backend/opengl/render_tasks/skeletal_animation_render_task.hpp>

#include <gl/command_execution.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>

#include <makina/renderer/backend/glsl/skeletal_animation_vertex_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<skeletal_animation_task_data>* add_skeletal_animation_render_task(renderer* framegraph, const upload_scene_task_data& scene_data)
{
  const auto retained_vertices = framegraph->add_retained_resource<buffer_description, gl::buffer>("Skeletal Animation Vertices", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER});
  const auto retained_normals  = framegraph->add_retained_resource<buffer_description, gl::buffer>("Skeletal Animation Normals" , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER});
  // Totals to 64 * 2 = 128 MB of GPU memory for buffers.

  return framegraph->add_render_task<skeletal_animation_task_data>(
    "Skeletal Animation Pass",
    [&] (      skeletal_animation_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices             = builder.read(scene_data.vertices     );
      data.normals              = builder.read(scene_data.normals      );
      data.bone_ids             = builder.read(scene_data.bone_ids     );
      data.bone_weights         = builder.read(scene_data.bone_weights );
      data.indices              = builder.read(scene_data.indices      );
      data.rigs                 = builder.read(scene_data.rigs         );
      data.draw_calls           = builder.read(scene_data.draw_calls   );
      data.parameter_map        = builder.read(scene_data.parameter_map);
      
      data.transformed_vertices = builder.write(retained_vertices);
      data.transformed_normals  = builder.write(retained_normals );
      data.transform_feedback   = builder.create<transform_feedback_resource>("Skeletal Animation Transform Feedback", transform_feedback_description {{data.transformed_vertices, data.transformed_normals}});

      data.program              = builder.create<program_resource>           ("Skeletal Animation Program"           , program::description     
      {
        glsl::skeletal_animation_vertex_shader
      });
      data.vertex_array         = builder.create<vertex_array_resource>      ("Skeletal Animation Vertex Array"      , vertex_array::description
      {
        { 
          {data.vertices    , 3, GL_FLOAT       }, 
          {data.normals     , 3, GL_FLOAT       }, 
          {data.bone_ids    , 4, GL_UNSIGNED_INT}, 
          {data.bone_weights, 4, GL_FLOAT       }
        }, 
        {
          data.rigs  
        }, 
        data.indices,
        data.draw_calls
      });
    },
    [=] (const skeletal_animation_task_data& data)
    {
      data.program           ->actual()->use   ();
      data.vertex_array      ->actual()->bind  ();
      data.transform_feedback->actual()->begin (GL_TRIANGLES);

      glClipControl                       (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled          (true   );
      gl::set_depth_function              (GL_LESS);
      gl::set_scissor_test_enabled        (false  );
      gl::set_polygon_face_culling_enabled(true   );
      gl::set_front_face                  (GL_CW  );
      gl::set_cull_face                   (GL_BACK);

      gl::set_rasterizer_discard_enabled  (true   );
      gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));
      gl::set_rasterizer_discard_enabled  (false  );

      data.transform_feedback->actual()->end   ();
      data.vertex_array      ->actual()->unbind();
      data.program           ->actual()->unuse ();

      gl::flush();

      gl::print_error("Error in Skeletal Animation Pass");
    });
}
}
}
