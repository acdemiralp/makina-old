#include <makina/renderer/backend/opengl/render_tasks.hpp>

#include <boost/lexical_cast.hpp>
#include <gl/all.hpp>

#include <makina/renderer/backend/glsl/shader_sources.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/material.hpp>
#include <makina/resources/mesh.hpp>

namespace mak
{
fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph)
{
  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [ ] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.create<buffer_resource>("Scene Vertices"           , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
      data.normals             = builder.create<buffer_resource>("Scene Normals"            , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
      data.texture_coordinates = builder.create<buffer_resource>("Scene Texture Coordinates", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
      data.instance_attributes = builder.create<buffer_resource>("Scene Instance Attributes", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
      data.indices             = builder.create<buffer_resource>("Scene Indices"            , buffer_description{GLsizeiptr(64e+6), GL_ELEMENT_ARRAY_BUFFER });
      data.transforms          = builder.create<buffer_resource>("Scene Transforms"         , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
      data.materials           = builder.create<buffer_resource>("Scene Materials"          , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
      data.cameras             = builder.create<buffer_resource>("Scene Cameras"            , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
      data.lights              = builder.create<buffer_resource>("Scene Lights"             , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
      // Totals to 64 * 5 + 16 * 4 = 384 MB of GPU memory for buffers.
      
      data.textures.resize(32);
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.create<texture_2d_resource>  ("Scene Texture " + boost::lexical_cast<std::string>(i), texture_description{{2048, 2048, 1}, GL_RGBA, 0});
      // Totals to 32 * 16.77 = 536 MB of GPU memory for textures.

      data.vertex_array  = builder.create<vertex_array_resource>("Scene Vertex Array", 
        vertex_array_description{
          { 
            {data.vertices           , 3, GL_FLOAT}, 
            {data.normals            , 3, GL_FLOAT}, 
            {data.texture_coordinates, 2, GL_FLOAT}, 
            {data.instance_attributes, 2, GL_UNSIGNED_INT}
          },
          {
            data.transforms, 
            data.materials , 
            data.cameras   , 
            data.lights
          }, data.indices});
    },
    [=] (const upload_scene_task_data& data)
    {
      auto scene = framegraph->scene_cache();      
      for (auto& entity : scene->entities<transform, projection> ())
      {
        const auto transform            = entity->component<mak::transform>  ();
        const auto projection           = entity->component<mak::projection> ();
      }
      for (auto& entity : scene->entities<transform, light>      ())
      {
        const auto transform            = entity->component<mak::transform>  ();
        const auto light                = entity->component<mak::light>      ();
      }
      for (auto& entity : scene->entities<transform, mesh_render>())
      {
        const auto transform            = entity->component<mak::transform>  ();
        const auto mesh_render          = entity->component<mak::mesh_render>();
    
        const auto& vertices            = mesh_render->mesh->vertices           ;
        const auto& normal              = mesh_render->mesh->normals            ;
        const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
        const auto& indices             = mesh_render->mesh->indices            ;
      }
    });
}
fg::render_task<clear_task_data>*        add_clear_render_task       (renderer* framegraph, framebuffer_resource* target, const glm::vec4&              color     )
{
  return framegraph->add_render_task<clear_task_data>(
    "Clear Pass",
    [&] (      clear_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.write(target);
    },
    [=] (const clear_task_data& data)
    {
      data.target->actual()->clear_color(std::array<float, 4>{color[0], color[1], color[2], color[3]});
      data.target->actual()->clear_depth_and_stencil(0.0f, 0);
    });
}
fg::render_task<phong_task_data>*        add_phong_render_task       (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  return framegraph->add_render_task<phong_task_data>(
    "Phong Shading Pass",
    [&] (      phong_task_data& data, fg::render_task_builder& builder)
    {
      data.program      = builder.create<program_resource>("Phong Shading Program", program::description{default_vertex_shader, phong_fragment_shader});
      data.vertex_array = builder.read (scene_data.vertex_array);
      data.target       = builder.write(target);
      data.textures.resize(scene_data.textures.size());
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.read(scene_data.textures[i]);

      gl::set_depth_test_enabled(true);
    },
    [=] (const phong_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();

      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();
    });
}
}