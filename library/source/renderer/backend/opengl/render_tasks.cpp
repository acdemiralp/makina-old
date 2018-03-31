#include <makina/renderer/backend/opengl/render_tasks.hpp>

#include <cstdint>
#include <optional>

#include <boost/lexical_cast.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <gl/auxiliary/glm_uniforms.hpp>
#include <gl/all.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <makina/renderer/backend/glsl/shader_sources.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>
#include <makina/resources/mesh.hpp>

namespace mak
{
fg::render_task<test_task_data>*                     add_test_render_task                    (renderer* framegraph, framebuffer_resource* target)
{
  return framegraph->add_render_task<test_task_data>(
    "Test Shading Pass",
    [&] (      test_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices     = builder.create<buffer_resource>      ("Test Shading Vertices"    , buffer_description        {GLsizeiptr(1e+6), GL_ARRAY_BUFFER});
      data.program      = builder.create<program_resource>     ("Test Shading Program"     , program::description      {test_vertex_shader, test_fragment_shader});
      data.vertex_array = builder.create<vertex_array_resource>("Test Shading Vertex Array", vertex_array::description {{{data.vertices, 3, GL_FLOAT}}});
      data.target       = builder.write(target);
    },
    [=] (const test_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();
      
      std::vector<glm::vec3> points = {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f}};
      data.vertices->actual()->set_sub_data(0, sizeof(glm::vec3) * points.size(), points.data());

      gl::draw_arrays(GL_TRIANGLE_STRIP, 0, 5);

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Test Shading Pass");
    });
}
fg::render_task<upload_scene_task_data>*             add_upload_scene_render_task            (renderer* framegraph)
{
  // Shader types with std430 alignment.
  struct _transform
  {
    glm::mat4    model         ;
  };
  struct _phong_material
  {
    glm::uvec4   use_texture   ; // ambient - diffuse - specular - unused
    glm::vec4    ambient       ; // w is unused.
    glm::vec4    diffuse       ; // w is unused.
    glm::vec4    specular      ; // w is shininess.
    glm::u64vec4 textures      ; // ambient - diffuse - specular - unused
  };
  struct _physically_based_material
  {
    glm::uvec4   use_texture   ; // albedo - metallicity - roughness - normal
    glm::uvec4   use_texture_2 ; // ambient occlusion - unused - unused - unused
    glm::vec4    albedo        ; // w is unused.
    glm::vec4    properties    ; // metallicity - roughness - unused - unused
    glm::u64vec4 textures      ; // albedo - metallicity - roughness - normal
    glm::u64vec4 textures_2    ; // ambient occlusion - unused - unused - unused
  };
  struct _camera
  {
    glm::mat4    view          ;
    glm::mat4    projection    ;
  };             
  struct _light  
  {              
    glm::uvec4   type          ; // y, z, w are unused.
    glm::vec4    color         ; // w is unused.
    glm::vec4    properties    ; // intensity - range - inner spot - outer spot
    glm::vec4    position      ; // w is unused.
    glm::vec4    direction     ; // w is unused.
  };

  const glm::uvec2 texture_size {2048, 2048};

  const auto retained_vertices            = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Vertices"           , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
  const auto retained_normals             = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Normals"            , buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
  const auto retained_texture_coordinates = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Texture Coordinates", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
  const auto retained_instance_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Instance Attributes", buffer_description{GLsizeiptr(64e+6), GL_ARRAY_BUFFER         });
  const auto retained_indices             = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Indices"            , buffer_description{GLsizeiptr(64e+6), GL_ELEMENT_ARRAY_BUFFER });
  const auto retained_transforms          = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Transforms"         , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
  const auto retained_materials           = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Materials"          , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
  const auto retained_cameras             = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Cameras"            , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
  const auto retained_lights              = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Lights"             , buffer_description{GLsizeiptr(16e+6), GL_SHADER_STORAGE_BUFFER});
  const auto retained_draw_calls          = framegraph->add_retained_resource<buffer_description, gl::buffer>("Scene Draw Calls"         , buffer_description{GLsizeiptr(16e+6), GL_DRAW_INDIRECT_BUFFER });
  // Totals to 64 * 5 + 16 * 5 = 400 MB of GPU memory for buffers.

  const auto retained_parameter_map       = framegraph->add_retained_resource<parameter_map::description, mak::parameter_map>("Scene Parameter Map", parameter_map::description());

  std::vector<texture_2d_resource*> retained_textures(64);
  for (auto i = 0; i < retained_textures.size(); ++i)
    retained_textures[i] = framegraph->add_retained_resource<texture_description, gl::texture_2d>("Scene Texture " + boost::lexical_cast<std::string>(i), texture_description {{static_cast<int>(texture_size[0]), static_cast<int>(texture_size[1]), 1}, GL_RGBA8});
  // Totals to 64 * 16.77 = 1073 MB of GPU memory for textures.

  // TODO: Optimize GPU uploads through scene caching.
  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [=] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.write<buffer_resource>       (retained_vertices           );
      data.normals             = builder.write<buffer_resource>       (retained_normals            );
      data.texture_coordinates = builder.write<buffer_resource>       (retained_texture_coordinates);
      data.instance_attributes = builder.write<buffer_resource>       (retained_instance_attributes);
      data.indices             = builder.write<buffer_resource>       (retained_indices            );
      data.transforms          = builder.write<buffer_resource>       (retained_transforms         );
      data.materials           = builder.write<buffer_resource>       (retained_materials          );
      data.cameras             = builder.write<buffer_resource>       (retained_cameras            );
      data.lights              = builder.write<buffer_resource>       (retained_lights             );
      data.draw_calls          = builder.write<buffer_resource>       (retained_draw_calls         );
      data.parameter_map       = builder.write<parameter_map_resource>(retained_parameter_map      );
      data.textures.resize(64);
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.write<texture_2d_resource>(retained_textures[i]);
    },
    [=] (const upload_scene_task_data& data)
    {
      auto scene                = framegraph->scene_cache();
      auto mesh_render_entities = scene->entities<transform, mesh_render>();
      auto light_entities       = scene->entities<transform, light>      ();
      auto camera_entities      = scene->entities<transform, projection> ();
      auto transforms           = std::vector<_transform>                        ();
      auto pbr_materials        = std::vector<_physically_based_material>        ();
      auto phong_materials      = std::vector<_phong_material>                   ();
      auto cameras              = std::vector<_camera>                           ();
      auto lights               = std::vector<_light>                            ();
      auto draw_calls           = std::vector<gl::draw_elements_indirect_command>();

      GLuint texture_offset     = 0;
      GLuint first_index_offset = 0;
      GLuint base_vertex_offset = 0;
      for (auto i = 0; i < mesh_render_entities.size(); ++i)
      {
        const auto& entity      = mesh_render_entities[i];
        const auto  transform   = entity->component<mak::transform>  ();
        const auto  mesh_render = entity->component<mak::mesh_render>();

        glm::vec3 texture_coordinate_scale(1.0f);

        auto pbr_material   = dynamic_cast<mak::physically_based_material*>(mesh_render->material);
        if  (pbr_material  )
        {
          std::optional<gl::texture_handle> albedo_handle, metallicity_handle, roughness_handle, normal_handle, ambient_occlusion_handle;
          if (pbr_material->albedo_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              pbr_material->albedo_image->dimensions()[0],
              pbr_material->albedo_image->dimensions()[1],
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              pbr_material->albedo_image->pixels().data);
            albedo_handle.emplace(*data.textures[texture_offset]->actual());
            if(!albedo_handle->is_resident())
              albedo_handle->set_resident(true);
            texture_offset++;
          }
          if (pbr_material->metallicity_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              pbr_material->metallicity_image->dimensions()[0],
              pbr_material->metallicity_image->dimensions()[1],
              GL_RED, 
              GL_UNSIGNED_BYTE, 
              pbr_material->metallicity_image->pixels().data);
            metallicity_handle.emplace(*data.textures[texture_offset]->actual());
            if(!metallicity_handle->is_resident())
              metallicity_handle->set_resident(true);
            texture_offset++;
          }
          if (pbr_material->roughness_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              pbr_material->roughness_image->dimensions()[0],
              pbr_material->roughness_image->dimensions()[1],
              GL_RED, 
              GL_UNSIGNED_BYTE, 
              pbr_material->roughness_image->pixels().data);
            roughness_handle.emplace(*data.textures[texture_offset]->actual());
            if(!roughness_handle->is_resident())
              roughness_handle->set_resident(true);
            texture_offset++;
          }
          if (pbr_material->normal_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              pbr_material->normal_image->dimensions()[0],
              pbr_material->normal_image->dimensions()[1],
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              pbr_material->normal_image->pixels().data);
            normal_handle.emplace(*data.textures[texture_offset]->actual());
            if(!normal_handle->is_resident())
              normal_handle->set_resident(true);
            texture_offset++;
          }
          if (pbr_material->ambient_occlusion_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              pbr_material->ambient_occlusion_image->dimensions()[0],
              pbr_material->ambient_occlusion_image->dimensions()[1],
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              pbr_material->ambient_occlusion_image->pixels().data);
            ambient_occlusion_handle.emplace(*data.textures[texture_offset]->actual());
            if(!ambient_occlusion_handle->is_resident())
              ambient_occlusion_handle->set_resident(true);
            texture_offset++;
          }

          pbr_materials.push_back(_physically_based_material {
            glm::uvec4 
            {
              static_cast<std::uint32_t>(albedo_handle           .has_value()),
              static_cast<std::uint32_t>(metallicity_handle      .has_value()),
              static_cast<std::uint32_t>(roughness_handle        .has_value()),
              static_cast<std::uint32_t>(normal_handle           .has_value())
            },
            glm::uvec4
            {
              static_cast<std::uint32_t>(ambient_occlusion_handle.has_value()), 0, 0, 0
            },
            glm::vec4   (pbr_material->albedo, 0.0f),
            glm::vec4   (pbr_material->metallicity, pbr_material->roughness, 0.0f, 0.0f),
            glm::u64vec4(
              albedo_handle            ? albedo_handle           ->id() : 0,
              metallicity_handle       ? metallicity_handle      ->id() : 0,
              roughness_handle         ? roughness_handle        ->id() : 0,
              normal_handle            ? normal_handle           ->id() : 0),
            glm::u64vec4(
              ambient_occlusion_handle ? ambient_occlusion_handle->id() : 0, 0, 0, 0)
          });

          if      (pbr_material->albedo_image )
            texture_coordinate_scale = glm::vec3(float(pbr_material->albedo_image           ->dimensions()[0]) / texture_size[0], float(pbr_material->albedo_image           ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (pbr_material->metallicity_image)
            texture_coordinate_scale = glm::vec3(float(pbr_material->metallicity_image      ->dimensions()[0]) / texture_size[0], float(pbr_material->metallicity_image      ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (pbr_material->roughness_image)
            texture_coordinate_scale = glm::vec3(float(pbr_material->roughness_image        ->dimensions()[0]) / texture_size[0], float(pbr_material->roughness_image        ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (pbr_material->normal_image)
            texture_coordinate_scale = glm::vec3(float(pbr_material->normal_image           ->dimensions()[0]) / texture_size[0], float(pbr_material->normal_image           ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (pbr_material->ambient_occlusion_image)
            texture_coordinate_scale = glm::vec3(float(pbr_material->ambient_occlusion_image->dimensions()[0]) / texture_size[0], float(pbr_material->ambient_occlusion_image->dimensions()[1]) / texture_size[1], 1.0f);
        }
        auto phong_material = dynamic_cast<mak::phong_material*>           (mesh_render->material);
        if  (phong_material)
        {
          std::optional<gl::texture_handle> ambient_handle, diffuse_handle, specular_handle;
          if (phong_material->ambient_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              phong_material->ambient_image->dimensions()[0], 
              phong_material->ambient_image->dimensions()[1], 
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              phong_material->ambient_image->pixels().data);
            ambient_handle.emplace(*data.textures[texture_offset]->actual());
            if(!ambient_handle->is_resident())
              ambient_handle->set_resident(true);
            texture_offset++;
          }
          if (phong_material->diffuse_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              phong_material->diffuse_image->dimensions()[0], 
              phong_material->diffuse_image->dimensions()[1], 
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              phong_material->diffuse_image->pixels().data);
            diffuse_handle.emplace(*data.textures[texture_offset]->actual());
            if(!diffuse_handle->is_resident())
              diffuse_handle->set_resident(true);
            texture_offset++;
          }
          if (phong_material->specular_image)
          {
            data.textures[texture_offset]->actual()->set_sub_image(0, 0, 0, 
              phong_material->specular_image->dimensions()[0], 
              phong_material->specular_image->dimensions()[1], 
              GL_BGRA, 
              GL_UNSIGNED_BYTE, 
              phong_material->specular_image->pixels().data);
            specular_handle.emplace(*data.textures[texture_offset]->actual());
            if(!specular_handle->is_resident())
              specular_handle->set_resident(true);
            texture_offset++;
          }

          phong_materials .push_back(_phong_material {
            glm::uvec4 
            {
              static_cast<std::uint32_t>(ambient_handle .has_value()),
              static_cast<std::uint32_t>(diffuse_handle .has_value()),
              static_cast<std::uint32_t>(specular_handle.has_value()),
              0
            },
            glm::vec4   (phong_material->ambient , 0.0f),
            glm::vec4   (phong_material->diffuse , 0.0f),
            glm::vec4   (phong_material->specular, phong_material->shininess),
            glm::u64vec4(
              ambient_handle  ? ambient_handle ->id() : 0, 
              diffuse_handle  ? diffuse_handle ->id() : 0, 
              specular_handle ? specular_handle->id() : 0, 
              0)
          });
          
          if      (phong_material->ambient_image )
            texture_coordinate_scale = glm::vec3(float(phong_material->ambient_image ->dimensions()[0]) / texture_size[0], float(phong_material->ambient_image ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (phong_material->diffuse_image )
            texture_coordinate_scale = glm::vec3(float(phong_material->diffuse_image ->dimensions()[0]) / texture_size[0], float(phong_material->diffuse_image ->dimensions()[1]) / texture_size[1], 1.0f);
          else if (phong_material->specular_image)
            texture_coordinate_scale = glm::vec3(float(phong_material->specular_image->dimensions()[0]) / texture_size[0], float(phong_material->specular_image->dimensions()[1]) / texture_size[1], 1.0f);
        }

        transforms.push_back(_transform {transform->matrix(true)});

        const auto& vertices            = mesh_render->mesh->vertices           ;
        const auto& normals             = mesh_render->mesh->normals            ;
        const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
        const auto& indices             = mesh_render->mesh->indices            ;
        const std::array<std::uint32_t, 2> instance_attribute {i, i};

        std::vector<glm::vec3> transformed_texture_coordinates(texture_coordinates.size());
        std::transform(texture_coordinates.begin(), texture_coordinates.end(), transformed_texture_coordinates.begin(), [texture_coordinate_scale] (const glm::vec3& texture_coordinate)
        {
          return texture_coordinate * texture_coordinate_scale;
        });

        data.vertices           ->actual()->set_sub_data(sizeof vertices           [0] * base_vertex_offset, sizeof vertices           [0] * vertices           .size(), vertices           .data());
        data.normals            ->actual()->set_sub_data(sizeof normals            [0] * base_vertex_offset, sizeof normals            [0] * normals            .size(), normals            .data());
        data.texture_coordinates->actual()->set_sub_data(sizeof texture_coordinates[0] * base_vertex_offset, sizeof texture_coordinates[0] * texture_coordinates.size(), transformed_texture_coordinates.data());
        data.indices            ->actual()->set_sub_data(sizeof indices            [0] * base_vertex_offset, sizeof indices            [0] * indices            .size(), indices            .data());
        data.instance_attributes->actual()->set_sub_data(sizeof std::array<std::uint32_t, 2> * i           , sizeof std::array<std::uint32_t, 2>                       , instance_attribute .data());

        draw_calls.push_back(gl::draw_elements_indirect_command
        {
          static_cast<GLuint>(indices.size()),
          1,
          first_index_offset,
          base_vertex_offset,
          static_cast<GLuint>(i)
        });

        first_index_offset += static_cast<GLuint>(indices .size());
        base_vertex_offset += static_cast<GLuint>(vertices.size());
      }
      for (auto& entity : camera_entities)
      {
        auto transform  = entity->component<mak::transform> ();
        auto projection = entity->component<mak::projection>();
        cameras.push_back(_camera
        {
          inverse(transform->matrix(true)),
          projection->matrix()
        });
      }     
      for (auto& entity : light_entities)
      {
        auto transform = entity->component<mak::transform>();
        auto light     = entity->component<mak::light>    ();
        lights.push_back(_light
        {
          glm::uvec4(light->type , 0, 0, 0),
          glm::vec4 (light->color, 0.0f),
          glm::vec4 (light->intensity, light->range, light->spot_angles[0], light->spot_angles[1]),
          glm::vec4 (transform->translation(true), 0.0f),
          glm::vec4 (transform->forward    (true), 0.0f)
        });
      }

      auto transforms_size = static_cast<GLuint>(transforms     .size());
      auto materials_size  = static_cast<GLuint>(pbr_materials  .size() > 0 ? pbr_materials.size() : phong_materials.size());
      auto cameras_size    = static_cast<GLuint>(cameras        .size());
      auto lights_size     = static_cast<GLuint>(lights         .size());
      data.transforms   ->actual()->set_sub_data(0               , sizeof GLuint                             , &transforms_size );
      data.materials    ->actual()->set_sub_data(0               , sizeof GLuint                             , &materials_size  );
      data.cameras      ->actual()->set_sub_data(0               , sizeof GLuint                             , &cameras_size    );
      data.lights       ->actual()->set_sub_data(0               , sizeof GLuint                             , &lights_size     );
      data.transforms   ->actual()->set_sub_data(sizeof glm::vec4, sizeof _transform                         * transforms_size  , transforms     .data());
      pbr_materials.size() > 0                                                                               
      ? data.materials  ->actual()->set_sub_data(sizeof glm::vec4, sizeof _physically_based_material         * materials_size   , pbr_materials  .data()) 
      : data.materials  ->actual()->set_sub_data(sizeof glm::vec4, sizeof _phong_material                    * materials_size   , phong_materials.data());
      data.cameras      ->actual()->set_sub_data(sizeof glm::vec4, sizeof _camera                            * cameras_size     , cameras        .data());
      data.lights       ->actual()->set_sub_data(sizeof glm::vec4, sizeof _light                             * lights_size      , lights         .data());
      data.draw_calls   ->actual()->set_sub_data(0               , sizeof gl::draw_elements_indirect_command * draw_calls.size(), draw_calls     .data());
      data.parameter_map->actual()->set         ("draw_count"    , draw_calls.size());

      gl::print_error("Error in Upload Scene Pass");
    });
}
fg::render_task<clear_task_data>*                    add_clear_render_task                   (renderer* framegraph, framebuffer_resource* target, const glm::vec4&              color     , const float depth)
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
      data.target->actual()->clear_depth_and_stencil(depth, 0);

      gl::print_error("Error in Clear Pass");
    });
}
fg::render_task<phong_task_data>*                    add_phong_render_task                   (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
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
      data.target       = builder.write(target);
    },
    [=] (const phong_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();

      glClipControl                       (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled          (true   );
      gl::set_depth_function              (GL_LESS);
      gl::set_scissor_test_enabled        (false  );
      gl::set_polygon_face_culling_enabled(true   );
      gl::set_front_face                  (GL_CW  );
      gl::set_cull_face                   (GL_BACK);
      gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Phong Shading Pass");
    });
}
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
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
      data.textures.resize(scene_data.textures.size());
      for (auto i = 0; i < data.textures.size(); ++i)
        data.textures[i] = builder.read(scene_data.textures[i]);

      data.program      = builder.create<program_resource>     ("Physically Based Shading Program"     , program::description     
      {
        default_vertex_shader, 
        physically_based_fragment_shader
      });
      data.vertex_array = builder.create<vertex_array_resource>("Physically Based Shading Vertex Array", vertex_array::description
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
      data.target       = builder.write(target);
    },
    [=] (const physically_based_shading_task_data& data)
    {
      data.program     ->actual()->use   ();
      data.vertex_array->actual()->bind  ();
      data.target      ->actual()->bind  ();

      glClipControl                       (GL_LOWER_LEFT, GL_ZERO_TO_ONE);
      gl::set_depth_test_enabled          (true   );
      gl::set_depth_function              (GL_LESS);
      gl::set_scissor_test_enabled        (false  );
      gl::set_polygon_face_culling_enabled(true   );
      gl::set_front_face                  (GL_CW  );
      gl::set_cull_face                   (GL_BACK);
      gl::multi_draw_elements_indirect    (GL_TRIANGLES, GL_UNSIGNED_INT, 0, data.parameter_map->actual()->get<GLsizei>("draw_count"));

      data.target      ->actual()->unbind();
      data.vertex_array->actual()->unbind();
      data.program     ->actual()->unuse ();

      gl::print_error("Error in Physically Based Shading Pass");
    });
}
fg::render_task<ui_task_data>*                       add_ui_render_task                      (renderer* framegraph, framebuffer_resource* target)
{
  const auto retained_attributes    = framegraph->add_retained_resource<buffer_description        , gl::buffer>    ("UI Vertices"     , buffer_description{GLsizeiptr(4e+6), GL_ARRAY_BUFFER         });
  const auto retained_indices       = framegraph->add_retained_resource<buffer_description        , gl::buffer>    ("UI Indices"      , buffer_description{GLsizeiptr(4e+6), GL_ELEMENT_ARRAY_BUFFER });
  const auto retained_parameter_map = framegraph->add_retained_resource<parameter_map::description, parameter_map> ("UI Parameter Map", parameter_map::description());

  auto& io = ImGui::GetIO();
  std::uint8_t* pixels;
  std::int32_t  width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  const auto retained_texture = framegraph->add_retained_resource<texture_description, gl::texture_2d>("UI Texture", texture_description {{static_cast<int>(width), static_cast<int>(height), 1}, GL_RGBA8});
  retained_texture->actual()->set_sub_image(0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  io.Fonts->TexID = reinterpret_cast<void*>(retained_texture->actual()->id());

  return framegraph->add_render_task<ui_task_data>(
    "UI Pass",
    [&] (      ui_task_data& data, fg::render_task_builder& builder)
    {
      data.attributes   = builder.read  <buffer_resource>      (retained_attributes);
      data.indices      = builder.read  <buffer_resource>      (retained_indices   );
      data.texture      = builder.read  <texture_2d_resource>  (retained_texture   );
      data.program      = builder.create<program_resource>     ("UI Program"     , program::description     
      {
        ui_vertex_shader, 
        ui_fragment_shader
      });
      data.vertex_array = builder.create<vertex_array_resource>("UI Vertex Array", vertex_array::description
      {
        { 
          {data.attributes, 2, GL_FLOAT},
          {data.attributes, 2, GL_FLOAT},
          {data.attributes, 4, GL_UNSIGNED_BYTE}
        }, 
        {}, 
        data.indices
      });
      data.target = builder.write(target);
    },
    [=] (const ui_task_data& data)
    {
      auto& io = ImGui::GetIO();

      auto program       = data.program     ->actual();
      auto vertex_array  = data.vertex_array->actual();
      auto target        = data.target      ->actual();

      program     ->use   ();
      vertex_array->bind  ();
      target      ->bind  ();

      gl::texture_handle handle(*data.texture->actual());
      if (!handle.is_resident()) handle.set_resident(true);

      program->set_uniform       (program->uniform_location("projection"), glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1.0f));
      program->set_uniform_handle(program->uniform_location("ui_texture"), handle);
      
      vertex_array->set_vertex_buffer   (0, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_vertex_buffer   (1, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_vertex_buffer   (2, *data.attributes->actual(), 0, sizeof ImDrawVert);
      vertex_array->set_attribute_format(0, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, pos));
      vertex_array->set_attribute_format(1, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, uv ));
      vertex_array->set_attribute_format(2, 4, GL_UNSIGNED_BYTE, true , IM_OFFSETOF(ImDrawVert, col));

      gl::set_blending_enabled            (true );
      gl::set_blend_function              (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      gl::set_polygon_face_culling_enabled(false);
      gl::set_depth_test_enabled          (false);
      gl::set_scissor_test_enabled        (true );
      
      ImGui::Render();
      auto draw_data = ImGui::GetDrawData();
      for(auto i = 0; i < draw_data->CmdListsCount; ++i)
      {
        const ImDrawList* command_list = draw_data->CmdLists[i];
        const ImDrawIdx*  index_offset = nullptr;
        data.attributes->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->VtxBuffer.Size) * sizeof(ImDrawVert), static_cast<const GLvoid*>(command_list->VtxBuffer.Data));
        data.indices   ->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->IdxBuffer.Size) * sizeof(ImDrawIdx) , static_cast<const GLvoid*>(command_list->IdxBuffer.Data));
        for(auto j = 0; j < command_list->CmdBuffer.Size; ++j)
        {
          auto& command = command_list->CmdBuffer[j];
          gl::set_scissor(
            {static_cast<int>(command.ClipRect.x)                     , static_cast<int>(io.DisplaySize.y   - command.ClipRect.w)},
            {static_cast<int>(command.ClipRect.z - command.ClipRect.x), static_cast<int>(command.ClipRect.w - command.ClipRect.y)});
          gl::draw_elements(GL_TRIANGLES, static_cast<GLsizei>(command.ElemCount), GL_UNSIGNED_SHORT, index_offset);
          index_offset += command.ElemCount;
        }
      }
      ImGui::NewFrame();

      gl::set_scissor_test_enabled(false);

      target      ->unbind();
      vertex_array->unbind();
      program     ->unuse ();

      gl::print_error("Error in UI Pass");
    });
}
}
