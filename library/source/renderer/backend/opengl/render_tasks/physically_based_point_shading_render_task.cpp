#include <makina/renderer/backend/opengl/render_tasks/physically_based_point_shading_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/point_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/point_physically_based_fragment_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<physically_based_point_shading_task_data>* add_physically_based_point_shading_render_task(
  renderer*                      framegraph , 
  framebuffer_resource*          target     , 
  const upload_common_task_data& common_data, 
  const upload_points_task_data& point_data , 
  const std::string&             camera_tag )
{
  const auto retained_program = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Physically Based Point Shading Program", program::graphics_description
  {
    glsl::point_vertex_shader,
    glsl::point_physically_based_fragment_shader
  });

  return framegraph->add_render_task<physically_based_point_shading_task_data>(
    "Physically Based Point Shading Pass",
    [&] (      physically_based_point_shading_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.read(point_data .vertices           );
      data.normals             = builder.read(point_data .normals            );
      data.colors              = builder.read(point_data .colors             );
      data.instance_attributes = builder.read(point_data .instance_attributes);
      data.draw_calls          = builder.read(point_data .draw_calls         );
      data.transforms          = builder.read(point_data .transforms         );
      data.materials           = builder.read(point_data .materials          );
      data.cameras             = builder.read(common_data.cameras            );
      data.lights              = builder.read(common_data.lights             );
      data.parameter_map       = builder.read(point_data .parameter_map      );
      data.program             = builder.read(retained_program               );
      data.vertex_array        = builder.create<vertex_array_resource>("Physically Based Point Shading Vertex Array", vertex_array::description
      {
        { 
          {data.vertices           , 4, GL_FLOAT                           }, 
          {data.normals            , 4, GL_FLOAT                           },
          {data.colors             , 4, GL_UNSIGNED_BYTE, true             },
          {data.instance_attributes, 2, GL_UNSIGNED_INT , false, 0, 0, 0, 1}
        }, 
        {
          data.transforms, 
          data.materials , 
          data.cameras   , 
          data.lights    
        }, 
        nullptr,
        data.draw_calls
      });
      data.target              = builder.write(target);
    },
    [=] (const physically_based_point_shading_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();

      if(!camera_tag.empty())
      {
        auto cameras = framegraph->scene_cache()->entities<metadata, projection>();
        for (std::uint32_t i = 0; i < cameras.size(); ++i)
          if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
            data.cameras->actual()->set_sub_data(sizeof glm::uint, sizeof glm::uint, &i);
      }
      
      glClipControl                 (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_blending_enabled      (true);
      gl::set_point_size_enabled    (true);
      gl::set_depth_function        (GL_LESS);
      gl::set_blend_equation        (GL_FUNC_ADD , GL_FUNC_ADD );
      gl::set_blend_function        (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      gl::set_viewport              ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      gl::multi_draw_arrays_indirect(GL_POINTS, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Physically Based Point Shading Pass");
    });
}
}
}