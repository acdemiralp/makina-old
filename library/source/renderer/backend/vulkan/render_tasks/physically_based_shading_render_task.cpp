#include <makina/renderer/backend/vulkan/render_tasks/physically_based_shading_render_task.hpp>

#include <makina/renderer/backend/glsl/default_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/physically_based_fragment_shader.hpp>

#include <vkhlf/DescriptorSet.h>

namespace mak
{
namespace vulkan
{
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  // Shader vertex type.
  struct _vertex
  {
    glm::vec3  position           ;
    glm::vec3  normal             ;
    glm::vec3  texture_coordinates;
    glm::uvec2 instance_attributes;
  };

  return framegraph->add_render_task<physically_based_shading_task_data>(
    "Physically Based Shading Pass",
    [&] (      physically_based_shading_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices      = builder.read(scene_data.vertices     );
      data.indices       = builder.read(scene_data.indices      );
      data.transforms    = builder.read(scene_data.transforms   );
      data.materials     = builder.read(scene_data.materials    );
      data.cameras       = builder.read(scene_data.cameras      );
      data.lights        = builder.read(scene_data.lights       );
      data.draw_calls    = builder.read(scene_data.draw_calls   );
      data.images        = builder.read(scene_data.images       );
      data.sampler       = builder.read(scene_data.sampler      );
      data.parameter_map = builder.read(scene_data.parameter_map);
      data.pipeline      = builder.create<pipeline_resource>("Physically Based Shading Pipeline", pipeline_description{
        glsl::default_vertex_shader,
        glsl::physically_based_fragment_shader,
        vk::PrimitiveTopology::eTriangleList,
        vulkan::context().window_swapchains[0].render_pass, // Applies to the first window swapchain.
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(_vertex, position           )),
          vk::VertexInputAttributeDescription(0, 1, vk::Format::eR32G32B32Sfloat, offsetof(_vertex, normal             )),
          vk::VertexInputAttributeDescription(0, 2, vk::Format::eR32G32B32Sfloat, offsetof(_vertex, texture_coordinates)),
          vk::VertexInputAttributeDescription(0, 3, vk::Format::eR32G32Uint     , offsetof(_vertex, instance_attributes))
        },
        sizeof _vertex});
      data.target        = builder.write(target);
      
      // TODO: Storage descriptors.
    },
    [=] (const physically_based_shading_task_data& data)
    {
      //data.program     ->actual()->use   ();
      //data.vertex_array->actual()->bind  ();
      //data.target      ->actual()->bind  ();
      //
      //gl::texture_handle handle(*data.textures->actual());
      //if (!handle.is_resident()) handle.set_resident(true);
      //data.program->actual()->set_uniform_handle(0, handle);
      //
      //glClipControl                       (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      //gl::set_depth_test_enabled          (true   );
      //gl::set_depth_function              (GL_LESS);
      //gl::set_scissor_test_enabled        (false  );
      //gl::set_polygon_face_culling_enabled(true   );
      //gl::set_front_face                  (GL_CW  );
      //gl::set_cull_face                   (GL_BACK);
      //gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));
      //
      //data.target      ->actual()->unbind();
      //data.vertex_array->actual()->unbind();
      //data.program     ->actual()->unuse ();
      //
      //gl::print_error("Error in Physically Based Shading Pass");
    });
}
}
}
