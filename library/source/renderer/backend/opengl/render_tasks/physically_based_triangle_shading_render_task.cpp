#include <makina/renderer/backend/opengl/render_tasks/physically_based_triangle_shading_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/triangle_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/triangle_physically_based_fragment_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<physically_based_triangle_shading_task_data>* add_physically_based_triangle_shading_render_task(
  renderer*                      framegraph , 
  framebuffer_resource*          target     , 
  const upload_common_task_data& common_data, 
  const upload_meshes_task_data& mesh_data  , 
  const std::string&             camera_tag )
{
  const auto retained_program = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Physically Based Triangle Shading Program", program::graphics_description
  {
    glsl::triangle_vertex_shader,
    glsl::triangle_physically_based_fragment_shader
  });

  return framegraph->add_render_task<physically_based_triangle_shading_task_data>(
    "Physically Based Triangle Shading Pass",
    [&] (      physically_based_triangle_shading_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.read(mesh_data  .vertices           );
      data.normals             = builder.read(mesh_data  .normals            );
      data.colors              = builder.read(mesh_data  .colors             );
      data.texture_coordinates = builder.read(mesh_data  .texture_coordinates);
      data.instance_attributes = builder.read(mesh_data  .instance_attributes);
      data.indices             = builder.read(mesh_data  .indices            );
      data.draw_calls          = builder.read(mesh_data  .draw_calls         );
      data.transforms          = builder.read(mesh_data  .transforms         );
      data.materials           = builder.read(mesh_data  .materials          );
      data.cameras             = builder.read(common_data.cameras            );
      data.lights              = builder.read(common_data.lights             );
      data.textures            = builder.read(mesh_data  .textures           );
      data.parameter_map       = builder.read(mesh_data  .parameter_map      );
      data.program             = builder.read(retained_program               );
      data.vertex_array        = builder.create<vertex_array_resource>("Physically Based Triangle Shading Vertex Array", vertex_array::description
      {
        { 
          {data.vertices           , 4, GL_FLOAT                           }, 
          {data.normals            , 4, GL_FLOAT                           },
          {data.colors             , 4, GL_UNSIGNED_BYTE, true             },
          {data.texture_coordinates, 3, GL_FLOAT                           }, 
          {data.instance_attributes, 2, GL_UNSIGNED_INT , false, 0, 0, 0, 1}
        }, 
        {
          data.transforms, 
          data.materials , 
          data.cameras   , 
          data.lights    
        }, 
        data.indices,
        data.draw_calls
      });
      data.target              = builder.write(target);
    },
    [=] (const physically_based_triangle_shading_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();

      gl::texture_handle handle(*data.textures->actual());
      if (!handle.is_resident()) handle.set_resident(true);
      data.program->actual()->set_uniform_handle(0, handle);
      
      if(!camera_tag.empty())
      {
        auto cameras = framegraph->scene_cache()->entities<metadata, projection>();
        for (std::uint32_t i = 0; i < cameras.size(); ++i)
          if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
            data.cameras->actual()->set_sub_data(sizeof glm::uint, sizeof glm::uint, &i);
      }
      
      glClipControl                       (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled          (true   );
      gl::set_depth_function              (GL_LESS);
      gl::set_scissor_test_enabled        (false  );
      gl::set_polygon_face_culling_enabled(true   );
      gl::set_front_face                  (GL_CW  );
      gl::set_cull_face                   (GL_BACK);
      gl::set_viewport                    ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Physically Based Triangle Shading Pass");
    });
}
}
}