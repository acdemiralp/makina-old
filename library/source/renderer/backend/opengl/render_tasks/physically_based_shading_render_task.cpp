#include <makina/renderer/backend/opengl/render_tasks/physically_based_shading_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/default_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/physically_based_fragment_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data, const std::string& camera_tag)
{
  return framegraph->add_render_task<physically_based_shading_task_data>(
    "Physically Based Shading Pass",
    [&] (      physically_based_shading_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.read(scene_data.vertices            );
      data.normals             = builder.read(scene_data.normals             );
      data.texture_coordinates = builder.read(scene_data.texture_coordinates );
      data.instance_attributes = builder.read(scene_data.instance_attributes );
      data.indices             = builder.read(scene_data.indices             );
      data.transforms          = builder.read(scene_data.transforms          );
      data.materials           = builder.read(scene_data.materials           );
      data.cameras             = builder.read(scene_data.cameras             );
      data.lights              = builder.read(scene_data.lights              );
      data.draw_calls          = builder.read(scene_data.draw_calls          );
      data.parameter_map       = builder.read(scene_data.parameter_map       );
      data.textures            = builder.read(scene_data.textures            );
      data.program             = builder.create<graphics_program_resource>("Physically Based Shading Program"     , program::graphics_description     
      {
        glsl::default_vertex_shader, 
        glsl::physically_based_fragment_shader
      });
      data.vertex_array        = builder.create<vertex_array_resource>    ("Physically Based Shading Vertex Array", vertex_array::description
      {
        { 
          {data.vertices           , 3, GL_FLOAT       }, 
          {data.normals            , 3, GL_FLOAT       }, 
          {data.texture_coordinates, 3, GL_FLOAT       }, 
          {data.instance_attributes, 2, GL_UNSIGNED_INT, false, 0, 0, 1}
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
    [=] (const physically_based_shading_task_data& data)
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

      gl::print_error("Error in Physically Based Shading Pass");
    });
}
}
}