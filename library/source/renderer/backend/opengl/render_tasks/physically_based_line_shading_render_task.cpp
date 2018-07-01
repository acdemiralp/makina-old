#include <makina/renderer/backend/opengl/render_tasks/physically_based_line_shading_render_task.hpp>

#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/viewport.hpp>

#include <makina/renderer/backend/glsl/line_geometry_shader.hpp>
#include <makina/renderer/backend/glsl/line_physically_based_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/line_vertex_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<physically_based_line_shading_task_data>* add_physically_based_line_shading_render_task(
  renderer*                      framegraph , 
  framebuffer_resource*          target     , 
  const upload_common_task_data& common_data, 
  const upload_lines_task_data&  line_data  , 
  const std::string&             camera_tag )
{
  const auto retained_program  = framegraph->add_retained_resource<graphics_program_resource::description_type, program>("Physically Based Line Shading Program", program::graphics_description
  {
    glsl::line_vertex_shader,
    glsl::line_physically_based_fragment_shader,
    glsl::line_geometry_shader
  });
  const auto retained_viewport = framegraph->add_retained_resource<buffer_description, gl::buffer>                      ("Physically Based Line Shading Viewport", buffer_description{sizeof(glm::vec2)});

  return framegraph->add_render_task<physically_based_line_shading_task_data>(
    "Physically Based Line Shading Pass",
    [&] (      physically_based_line_shading_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.read(line_data  .vertices           );
      data.colors              = builder.read(line_data  .colors             );
      data.instance_attributes = builder.read(line_data  .instance_attributes);
      data.indices             = builder.read(line_data  .indices            );
      data.draw_calls          = builder.read(line_data  .draw_calls         );
      data.transforms          = builder.read(line_data  .transforms         );
      data.materials           = builder.read(line_data  .materials          );
      data.cameras             = builder.read(common_data.cameras            );
      data.lights              = builder.read(common_data.lights             );
      data.parameter_map       = builder.read(line_data  .parameter_map      );
      data.viewport            = builder.read(retained_viewport              );
      data.program             = builder.read(retained_program               );
      data.vertex_array        = builder.create<vertex_array_resource>("Physically Based Line Shading Vertex Array", vertex_array::description
      {
        { 
          {data.vertices           , 4, GL_FLOAT                           }, 
          {data.colors             , 4, GL_UNSIGNED_BYTE, true             },
          {data.instance_attributes, 2, GL_UNSIGNED_INT , false, 0, 0, 0, 1}
        }, 
        {
          data.transforms, 
          data.materials , 
          data.cameras   , 
          data.lights    ,
          data.viewport
        }, 
        data.indices,
        data.draw_calls
      });
      data.target              = builder.write(target);
    },
    [=] (const physically_based_line_shading_task_data& data)
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
      
      glm::vec2 size {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()};
      data.viewport->actual()->set_sub_data(0, sizeof glm::vec2, &size[0]);
      
      glClipControl                   (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled      (true   );
      gl::set_blending_enabled        (true   );
      gl::set_depth_function          (GL_LESS);
      gl::set_blend_equation          (GL_FUNC_ADD, GL_FUNC_ADD);
      gl::set_blend_function          (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      gl::set_viewport                ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
      gl::multi_draw_elements_indirect(GL_LINES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Physically Based Line Shading Pass");
    });
}
}
}