#include <makina/renderer/backend/opengl/render_tasks.hpp>

#include <boost/lexical_cast.hpp>
#include <gl/all.hpp>

#include <makina/renderer/backend/glsl/shader_sources.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/mesh.hpp>

namespace mak
{
fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph)
{
  // Shader types.
  struct _transform
  {
    glm::mat4  model           ;
  };
  struct _material
  {
    glm::bvec3 use_texture     ;
    glm::vec3  ambient         ;
    glm::vec3  diffuse         ;
    glm::vec3  specular        ;
    glm::f32   shininess       ;
    glm::u64   ambient_texture ;
    glm::u64   diffuse_texture ;
    glm::u64   specular_texture;
  };
  struct _camera
  {
    glm::mat4  view            ;
    glm::mat4  projection      ;
  };
  struct _light
  {            
    glm::uint  type            ;
    glm::vec3  color           ;
    glm::f32   intensity       ;
    glm::f32   range           ; // spot-point
    glm::vec2  spot_angles     ; // spot
    glm::vec3  position        ; // spot-point
    glm::vec3  direction       ; // spot-directional
  };

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
      data.draw_calls          = builder.create<buffer_resource>("Scene Draw Calls"         , buffer_description{GLsizeiptr(16e+6), GL_DRAW_INDIRECT_BUFFER });
      // Totals to 64 * 5 + 16 * 5 = 400 MB of GPU memory for buffers.

      data.parameter_map = builder.create<parameter_map_resource>("Scene Parameter Map", parameter_map::description());
      
      data.textures.resize(32);
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.create<texture_2d_resource>("Scene Texture " + boost::lexical_cast<std::string>(i), texture_description{{2048, 2048, 1}, GL_RGBA, 0});
      // Totals to 32 * 16.77 = 536 MB of GPU memory for textures.
    },
    [=] (const upload_scene_task_data& data)
    {
      auto scene                = framegraph->scene_cache();
      auto mesh_render_entities = scene->entities<transform, mesh_render>();
      auto light_entities       = scene->entities<transform, light>      ();
      auto camera_entities      = scene->entities<transform, projection> ();
      auto transforms           = std::vector<_transform>();
      auto materials            = std::vector<_material> ();
      auto cameras              = std::vector<_camera>   ();
      auto lights               = std::vector<_light>    ();

      for(auto& entity : mesh_render_entities)
      {
        auto transform = entity->component<mak::transform>  ();
        auto material  = dynamic_cast<phong_material*>(entity->component<mak::mesh_render>()->material);
        if (!material) continue;

        transforms.push_back(_transform {transform->matrix(true)});

        materials .push_back(_material  {
          glm::bvec3 
          {
            material->ambient_image  != nullptr, 
            material->diffuse_image  != nullptr, 
            material->specular_image != nullptr
          },
          material->ambient  ,
          material->diffuse  ,
          material->specular ,
          material->shininess,
          42, // TODO Load textures first. Pass IDs here.
          42, // TODO Load textures first. Pass IDs here.
          42  // TODO Load textures first. Pass IDs here.
        });
      }      
      for(auto& entity : camera_entities)
      {
        auto transform  = entity->component<mak::transform> ();
        auto projection = entity->component<mak::projection>();
        cameras.push_back(_camera
        {
          inverse(transform->matrix(true)),
          projection->matrix()
        });
      }
      for(auto& entity : light_entities)
      {
        auto transform = entity->component<mak::transform>();
        auto light     = entity->component<mak::light>    ();
        lights.push_back(_light
        {
          static_cast<GLuint>(light->type),
          light    ->color                ,
          light    ->intensity            ,
          light    ->range                ,
          light    ->spot_angles          ,
          transform->translation(true)    ,
          transform->forward    (true)
        });
      }

      auto transforms_size = static_cast<GLuint>(transforms.size());
      auto materials_size  = static_cast<GLuint>(materials .size());
      auto cameras_size    = static_cast<GLuint>(cameras   .size());
      auto lights_size     = static_cast<GLuint>(lights    .size());
      data.transforms->actual()->set_sub_data(0, sizeof glm::uint, &transforms_size);
      data.materials ->actual()->set_sub_data(0, sizeof glm::uint, &materials_size );
      data.cameras   ->actual()->set_sub_data(0, sizeof glm::uint, &cameras_size   );
      data.lights    ->actual()->set_sub_data(0, sizeof glm::uint, &lights_size    );
      data.transforms->actual()->set_sub_data(sizeof glm::uint, sizeof(glm::mat4) * transforms.size(), transforms.data());
      data.materials ->actual()->set_sub_data(sizeof glm::uint, sizeof(glm::mat4) * materials .size(), materials .data());
      data.cameras   ->actual()->set_sub_data(sizeof glm::uint, sizeof(glm::mat4) * cameras   .size(), cameras   .data());
      data.lights    ->actual()->set_sub_data(sizeof glm::uint, sizeof(glm::mat4) * lights    .size(), lights    .data());

      GLuint first_index_offset = 0;
      GLuint base_vertex_offset = 0;
      std::vector<gl::draw_elements_indirect_command> draw_calls;
      for (auto i = 0; i < mesh_render_entities.size(); ++i)
      {
        const auto& entity              = mesh_render_entities[i];
        const auto  transform           = entity->component<mak::transform>  ();
        const auto  mesh_render         = entity->component<mak::mesh_render>();

        const auto& vertices            = mesh_render->mesh->vertices           ;
        const auto& normals             = mesh_render->mesh->normals            ;
        const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
        const auto& indices             = mesh_render->mesh->indices            ;
        const auto& material            = mesh_render->material                 ;
        
        const std::array<std::uint32_t, 2> instance_attribute {i, i}; // TODO Transform index, material index.

        data.vertices           ->actual()->set_sub_data(sizeof vertices           [0] * base_vertex_offset, sizeof vertices           [0] * vertices           .size(), vertices           .data());
        data.normals            ->actual()->set_sub_data(sizeof normals            [0] * base_vertex_offset, sizeof normals            [0] * normals            .size(), normals            .data());
        data.texture_coordinates->actual()->set_sub_data(sizeof texture_coordinates[0] * base_vertex_offset, sizeof texture_coordinates[0] * texture_coordinates.size(), texture_coordinates.data());
        data.instance_attributes->actual()->set_sub_data(sizeof std::array<std::uint32_t, 2> * i           , sizeof std::array<std::uint32_t, 2>                       , instance_attribute .data());
        data.indices            ->actual()->set_sub_data(sizeof indices            [0] * base_vertex_offset, sizeof indices            [0] * indices            .size(), indices            .data());

        draw_calls.push_back(gl::draw_elements_indirect_command
        {
          static_cast<GLuint>(indices .size()),
          1,
          first_index_offset,
          base_vertex_offset,
          static_cast<GLuint>(i)
        });

        first_index_offset += static_cast<GLuint>(indices .size());
        base_vertex_offset += static_cast<GLuint>(vertices.size());
      }
      data.draw_calls   ->actual()->set_sub_data(0, sizeof draw_calls[0] * draw_calls.size(), draw_calls.data());
      data.parameter_map->actual()->set         ("draw_count", draw_calls.size());
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
      data.textures.resize(scene_data.textures.size());
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.read(scene_data.textures[i]);

      data.program      = builder.create<program_resource>     ("Phong Shading Program"     , program::description     
      {
        default_vertex_shader, 
        phong_fragment_shader
      });
      data.vertex_array = builder.create<vertex_array_resource>("Phong Shading Vertex Array", vertex_array::description
      {
        { 
          {data.vertices           , 3, GL_FLOAT       }, 
          {data.normals            , 3, GL_FLOAT       }, 
          {data.texture_coordinates, 2, GL_FLOAT       }, 
          {data.instance_attributes, 2, GL_UNSIGNED_INT, false, 1}
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
      data.target       = builder.write(target);
    },
    [=] (const phong_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();
      
      gl::set_depth_test_enabled          (true   );
      gl::set_polygon_face_culling_enabled(true   );
      gl::set_front_face                  (GL_CW  );
      gl::set_cull_face                   (GL_BACK);
      gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));
      
      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();
    });
}
}