#include <makina/renderer/backend/opengl/render_tasks/upload_scene_render_task.hpp>

#include <gl/texture_handle.hpp>

#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace opengl
{
// Shader types with std430 alignment.
struct _transform
{
  glm::mat4  model         ;
};
struct _phong_material
{
  glm::uvec4 use_texture   ; // ambient - diffuse - specular - unused
  glm::vec4  ambient       ; // w is unused.
  glm::vec4  diffuse       ; // w is unused.
  glm::vec4  specular      ; // w is shininess.
  glm::uvec4 texture_ids   ; // ambient - diffuse - specular - unused
};
struct _physically_based_material
{
  glm::uvec4 use_texture   ; // albedo - metallicity - roughness - normal
  glm::uvec4 use_texture_2 ; // ambient occlusion - unused - unused - unused
  glm::vec4  albedo        ; // w is unused.
  glm::vec4  properties    ; // metallicity - roughness - unused - unused
  glm::uvec4 textures_ids  ; // albedo - metallicity - roughness - normal
  glm::uvec4 textures_ids_2; // ambient occlusion - unused - unused - unused
};
struct _camera
{
  glm::mat4  view          ;
  glm::mat4  projection    ;
};           
struct _light
{            
  glm::uvec4 type          ; // y, z, w are unused.
  glm::vec4  color         ; // w is unused.
  glm::vec4  properties    ; // intensity - range - inner spot - outer spot
  glm::vec4  position      ; // w is unused.
  glm::vec4  direction     ; // w is unused.
};
struct _rig
{
  glm::mat4  model         ;
  glm::mat4  offset        ;
};

fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph)
{
  const glm::uvec3 texture_size {2048, 2048, 32};
  
  const auto retained_parameter_map       = framegraph->add_retained_resource<parameter_map::description, parameter_map>("Scene Parameter Map"      , parameter_map::description());

  const auto retained_vertices            = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Vertices"           , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_normals             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Normals"            , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_colors              = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Colors"             , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_texture_coordinates = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Texture Coordinates", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_bone_ids            = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Bone Ids"           , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_bone_weights        = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Bone Weights"       , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_instance_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Instance Attributes", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_indices             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Indices"            , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_transforms          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Transforms"         , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_materials           = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Materials"          , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_cameras             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Cameras"            , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_lights              = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Lights"             , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_rigs                = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Rigs"               , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_draw_calls          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Scene Draw Calls"         , buffer_description{GLsizeiptr(16e+6)});
  // Totals to 64 * 8 + 16 * 6 = 608 MB of GPU memory for buffers.

  const auto retained_texture_array       = framegraph->add_retained_resource<texture_description, gl::texture_2d_array>("Scene Texture Array"      , texture_description{{static_cast<int>(texture_size[0]), static_cast<int>(texture_size[1]), static_cast<int>(texture_size[2])}, GL_RGBA8});
  // Totals to 32 * 16.77 = 536.64 MB of GPU memory for textures.

  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [=] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.write<buffer_resource>          (retained_vertices           );
      data.normals             = builder.write<buffer_resource>          (retained_normals            );
      data.colors              = builder.write<buffer_resource>          (retained_colors             );
      data.texture_coordinates = builder.write<buffer_resource>          (retained_texture_coordinates);
      data.bone_ids            = builder.write<buffer_resource>          (retained_bone_ids           );
      data.bone_weights        = builder.write<buffer_resource>          (retained_bone_weights       );
      data.instance_attributes = builder.write<buffer_resource>          (retained_instance_attributes);
      data.indices             = builder.write<buffer_resource>          (retained_indices            );
      data.transforms          = builder.write<buffer_resource>          (retained_transforms         );
      data.materials           = builder.write<buffer_resource>          (retained_materials          );
      data.cameras             = builder.write<buffer_resource>          (retained_cameras            );
      data.lights              = builder.write<buffer_resource>          (retained_lights             );
      data.rigs                = builder.write<buffer_resource>          (retained_rigs               );
      data.draw_calls          = builder.write<buffer_resource>          (retained_draw_calls         );
      data.parameter_map       = builder.write<parameter_map_resource>   (retained_parameter_map      );
      data.textures            = builder.write<texture_2d_array_resource>(retained_texture_array      );
    },
    [=] (const upload_scene_task_data& data)
    {
      auto& mutable_data          = const_cast<upload_scene_task_data&>(data);
                                 
      auto scene                  = framegraph->scene_cache();
      auto line_render_entities   = scene->entities<transform, line_render>        ();
      auto mesh_render_entities   = scene->entities<transform, mesh_render>        ();
      auto point_render_entities  = scene->entities<transform, point_render>       ();
      auto volume_render_entities = scene->entities<transform, volume_render>      ();
      auto light_entities         = scene->entities<transform, light>              ();
      auto camera_entities        = scene->entities<transform, projection>         ();
      auto transforms             = std::vector<_transform>                        ();
      auto pbr_materials          = std::vector<_physically_based_material>        ();
      auto phong_materials        = std::vector<_phong_material>                   ();
      auto cameras                = std::vector<_camera>                           ();
      auto lights                 = std::vector<_light>                            ();
      auto rigs                   = std::vector<_rig>                              ();
      auto draw_calls             = std::vector<gl::draw_elements_indirect_command>();
      auto instance_attributes    = std::vector<glm::uvec2>                        ();
      
      for (auto i = 0; i < mesh_render_entities  .size(); ++i)
      {
        const auto& entity      = mesh_render_entities[i];
        const auto  animator    = entity->component<mak::animator>   ();
        const auto  transform   = entity->component<mak::transform>  ();
        const auto  metadata    = entity->component<mak::metadata>   ();
        const auto  mesh_render = entity->component<mak::mesh_render>();
        
        if (!metadata->active) continue;

        auto texture_coordinate_scale = glm::vec3(1.0f);
        auto pbr_material             = dynamic_cast<mak::physically_based_material*>(mesh_render->material);
        if  (pbr_material)
        {
          std::optional<std::uint32_t> albedo_offset, metallicity_offset, roughness_offset, normal_offset, ambient_occlusion_offset;
          if (pbr_material->albedo_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->albedo_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                pbr_material->albedo_image->dimensions()[0],
                pbr_material->albedo_image->dimensions()[1],
                1,
                GL_BGRA, 
                GL_UNSIGNED_BYTE, 
                pbr_material->albedo_image->pixels().data);
              mutable_data.image_cache[pbr_material->albedo_image.get()] = mutable_data.image_offset++;
            }
            albedo_offset = mutable_data.image_cache[pbr_material->albedo_image.get()];
          }
          if (pbr_material->metallicity_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->metallicity_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                pbr_material->metallicity_image->dimensions()[0],
                pbr_material->metallicity_image->dimensions()[1],
                1,
                GL_RED,
                GL_UNSIGNED_BYTE,
                pbr_material->metallicity_image->pixels().data);
              mutable_data.image_cache[pbr_material->metallicity_image.get()] = mutable_data.image_offset++;
            }
            metallicity_offset = mutable_data.image_cache[pbr_material->metallicity_image.get()];
          }
          if (pbr_material->roughness_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->roughness_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                pbr_material->roughness_image->dimensions()[0],
                pbr_material->roughness_image->dimensions()[1],
                1,
                GL_RED, 
                GL_UNSIGNED_BYTE, 
                pbr_material->roughness_image->pixels().data);
              mutable_data.image_cache[pbr_material->roughness_image.get()] = mutable_data.image_offset++;
            }
            roughness_offset = mutable_data.image_cache[pbr_material->roughness_image.get()];
          }
          if (pbr_material->normal_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->normal_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                pbr_material->normal_image->dimensions()[0],
                pbr_material->normal_image->dimensions()[1],
                1,
                GL_BGRA,
                GL_UNSIGNED_BYTE,
                pbr_material->normal_image->pixels().data);
              mutable_data.image_cache[pbr_material->normal_image.get()] = mutable_data.image_offset++;
            }
            normal_offset = mutable_data.image_cache[pbr_material->normal_image.get()];
          }
          if (pbr_material->ambient_occlusion_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->ambient_occlusion_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                pbr_material->ambient_occlusion_image->dimensions()[0],
                pbr_material->ambient_occlusion_image->dimensions()[1],
                1,
                GL_BGRA, 
                GL_UNSIGNED_BYTE, 
                pbr_material->ambient_occlusion_image->pixels().data);
              mutable_data.image_cache[pbr_material->ambient_occlusion_image.get()] = mutable_data.image_offset++;
            }
            ambient_occlusion_offset = mutable_data.image_cache[pbr_material->ambient_occlusion_image.get()];
          }

          pbr_materials.push_back(_physically_based_material {
            glm::uvec4 
            {
              static_cast<std::uint32_t>(albedo_offset     .has_value()),
              static_cast<std::uint32_t>(metallicity_offset.has_value()),
              static_cast<std::uint32_t>(roughness_offset  .has_value()),
              static_cast<std::uint32_t>(normal_offset     .has_value())
            },
            glm::uvec4
            {
              static_cast<std::uint32_t>(ambient_occlusion_offset.has_value()), 0, 0, 0
            },
            glm::vec4 (pbr_material->albedo, 0.0f),
            glm::vec4 (pbr_material->metallicity, pbr_material->roughness, 0.0f, 0.0f),
            glm::uvec4(
              albedo_offset            ? *albedo_offset            : 0,
              metallicity_offset       ? *metallicity_offset       : 0,
              roughness_offset         ? *roughness_offset         : 0,
              normal_offset            ? *normal_offset            : 0),
            glm::uvec4(
              ambient_occlusion_offset ? *ambient_occlusion_offset : 0, 0, 0, 0)
          });

          if      (pbr_material->albedo_image)
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
        auto phong_material           = dynamic_cast<mak::phong_material*>           (mesh_render->material);
        if  (phong_material)
        {
          std::optional<std::uint32_t> ambient_offset, diffuse_offset, specular_offset;
          if (phong_material->ambient_image)
          {
            if (!mutable_data.image_cache.count(phong_material->ambient_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                phong_material->ambient_image->dimensions()[0], 
                phong_material->ambient_image->dimensions()[1], 
                1,
                GL_BGRA, 
                GL_UNSIGNED_BYTE, 
                phong_material->ambient_image->pixels().data);
              mutable_data.image_cache[phong_material->ambient_image.get()] = mutable_data.image_offset++;
            }
            ambient_offset = mutable_data.image_cache[phong_material->ambient_image.get()];
          }
          if (phong_material->diffuse_image)
          {
            if (!mutable_data.image_cache.count(phong_material->diffuse_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                phong_material->diffuse_image->dimensions()[0], 
                phong_material->diffuse_image->dimensions()[1], 
                1,
                GL_BGRA, 
                GL_UNSIGNED_BYTE, 
                phong_material->diffuse_image->pixels().data);
              mutable_data.image_cache[phong_material->diffuse_image.get()] = mutable_data.image_offset++;
            }
            diffuse_offset = mutable_data.image_cache[phong_material->diffuse_image.get()];
          }
          if (phong_material->specular_image)
          {
            if (!mutable_data.image_cache.count(phong_material->specular_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, mutable_data.image_offset,
                phong_material->specular_image->dimensions()[0], 
                phong_material->specular_image->dimensions()[1], 
                1,
                GL_BGRA, 
                GL_UNSIGNED_BYTE, 
                phong_material->specular_image->pixels().data);
              mutable_data.image_cache[phong_material->specular_image.get()] = mutable_data.image_offset++;
            }
            specular_offset = mutable_data.image_cache[phong_material->specular_image.get()];
          }

          phong_materials .push_back(_phong_material {
            glm::uvec4 
            {
              static_cast<std::uint32_t>(ambient_offset .has_value()),
              static_cast<std::uint32_t>(diffuse_offset .has_value()),
              static_cast<std::uint32_t>(specular_offset.has_value()),
              0
            },
            glm::vec4   (phong_material->ambient , 0.0f),
            glm::vec4   (phong_material->diffuse , 0.0f),
            glm::vec4   (phong_material->specular, phong_material->shininess),
            glm::u64vec4(
              ambient_offset  ? *ambient_offset  : 0,
              diffuse_offset  ? *diffuse_offset  : 0,
              specular_offset ? *specular_offset : 0,
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

        if (!mutable_data.mesh_cache.count(mesh_render->mesh))
        {
          const auto& vertices            = mesh_render->mesh->vertices           ;
          const auto& normals             = mesh_render->mesh->normals            ;
          const auto& colors              = mesh_render->mesh->colors             ;
          const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
          const auto& bone_ids            = mesh_render->mesh->bone_ids           ;
          const auto& bone_weights        = mesh_render->mesh->bone_weights       ;
          const auto& indices             = mesh_render->mesh->indices            ;
          
          std::vector<glm::vec4> transformed_vertices(vertices.size());
          std::transform(
            vertices            .begin(), 
            vertices            .end  (), 
            transformed_vertices.begin(), 
            [ ] (const glm::vec3& vertex)
            {
              return glm::vec4(vertex, 1.0f);
            });
          
          std::vector<glm::vec4> transformed_normals(normals.size());
          std::transform(
            normals            .begin(), 
            normals            .end  (), 
            transformed_normals.begin(), 
            [ ] (const glm::vec3& normal)
            {
              return glm::vec4(normal, 0.0f);
            });

          std::vector<glm::vec3> transformed_texture_coordinates(texture_coordinates.size());
          std::transform(
            texture_coordinates            .begin(), 
            texture_coordinates            .end  (), 
            transformed_texture_coordinates.begin(), 
            [texture_coordinate_scale] (const glm::vec3& texture_coordinate)
            {
              return texture_coordinate * texture_coordinate_scale;
            });
          
          std::vector<glm::uvec4> transformed_bone_ids(bone_ids.size());
          std::transform(
            bone_ids            .begin(), 
            bone_ids            .end  (), 
            transformed_bone_ids.begin(), 
            [mutable_data] (const glm::uvec4& bone_id)
            {
              return bone_id + glm::uvec4(mutable_data.rig_offset);
            });

          data.vertices           ->actual()->set_sub_data(sizeof transformed_vertices           [0] * mutable_data.vertex_offset, sizeof transformed_vertices           [0] * transformed_vertices           .size(), transformed_vertices           .data());
          data.normals            ->actual()->set_sub_data(sizeof transformed_normals            [0] * mutable_data.vertex_offset, sizeof transformed_normals            [0] * transformed_normals            .size(), transformed_normals            .data());
          data.colors             ->actual()->set_sub_data(sizeof colors                         [0] * mutable_data.vertex_offset, sizeof colors                         [0] * colors                         .size(), colors                         .data());
          data.texture_coordinates->actual()->set_sub_data(sizeof transformed_texture_coordinates[0] * mutable_data.vertex_offset, sizeof transformed_texture_coordinates[0] * transformed_texture_coordinates.size(), transformed_texture_coordinates.data());
          data.bone_ids           ->actual()->set_sub_data(sizeof transformed_bone_ids           [0] * mutable_data.vertex_offset, sizeof transformed_bone_ids           [0] * transformed_bone_ids           .size(), transformed_bone_ids           .data());
          data.bone_weights       ->actual()->set_sub_data(sizeof bone_weights                   [0] * mutable_data.vertex_offset, sizeof bone_weights                   [0] * bone_weights                   .size(), bone_weights                   .data());
          data.indices            ->actual()->set_sub_data(sizeof indices                        [0] * mutable_data.index_offset , sizeof indices                        [0] * indices                        .size(), indices                        .data());

          auto& draw_call = mutable_data.mesh_cache[mesh_render->mesh];
          draw_call = gl::draw_elements_indirect_command
          {
            static_cast<GLuint>(indices.size()),
            1,
            mutable_data.index_offset ,
            mutable_data.vertex_offset,
            0 // Set per-instance.
          };
          
          mutable_data.vertex_offset += static_cast<GLuint>(vertices.size());
          mutable_data.index_offset  += static_cast<GLuint>(indices .size());
        }

        auto draw_call          = mutable_data.mesh_cache[mesh_render->mesh];
        draw_call.base_instance = i;
        draw_calls         .push_back(draw_call);
        instance_attributes.push_back({i, i});

        if (animator)
        {
          if (!mutable_data.rig_cache.count(animator))
          {
            auto& transform_bone_pairs = mutable_data.rig_cache[animator];
            std::function<void(mak::transform*)> recursive_collect = [&] (mak::transform* transform)
            {
              if(transform->metadata()->entity->has_components<mak::bone>())
                transform_bone_pairs.push_back({transform, transform->metadata()->entity->component<mak::bone>()});
              for (auto child : transform->children())
                recursive_collect(child);
            };
            recursive_collect(animator->root_transform);

            std::sort(
              transform_bone_pairs.begin(), 
              transform_bone_pairs.end  (), 
              [ ] (const std::pair<mak::transform*, mak::bone*>& lhs, const std::pair<mak::transform*, mak::bone*>& rhs)
              {
                return lhs.second->index < rhs.second->index;
              });

            mutable_data.rig_offset += static_cast<GLuint>(transform_bone_pairs.size());
          }
          for (auto& transform_bone_pair : mutable_data.rig_cache[animator])
            rigs.push_back(_rig{transform_bone_pair.first->matrix(true), transform_bone_pair.second->offset_matrix});
        }
      }
      for (auto i = 0; i < line_render_entities  .size(); ++i)
      {
        const auto& entity        = line_render_entities  [i];
        const auto  metadata      = entity->component<mak::metadata>     ();
        const auto  transform     = entity->component<mak::transform>    ();
        const auto  line_render   = entity->component<mak::line_render>  ();

        if (!metadata->active) continue;

        // TODO.
      }                                          
      for (auto i = 0; i < point_render_entities .size(); ++i)
      {
        const auto& entity        = point_render_entities [i];
        const auto  metadata      = entity->component<mak::metadata>     ();
        const auto  transform     = entity->component<mak::transform>    ();
        const auto  point_render  = entity->component<mak::point_render> ();

        if (!metadata->active) continue;

        // TODO.
      }
      for (auto i = 0; i < volume_render_entities.size(); ++i)
      {
        const auto& entity        = volume_render_entities[i];
        const auto  metadata      = entity->component<mak::metadata>     ();
        const auto  transform     = entity->component<mak::transform>    ();
        const auto  volume_render = entity->component<mak::volume_render>();

        if (!metadata->active) continue;

        // TODO.
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
      for (auto& entity : light_entities )
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
      
      auto transforms_metadata = glm::uvec4(transforms   .size(), 0u, 0u, 0u);
      auto materials_metadata  = glm::uvec4(pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size(), 0u, 0u, 0u);
      auto cameras_metadata    = glm::uvec4(cameras      .size(), 0u, 0u, 0u);
      auto lights_metadata     = glm::uvec4(lights       .size(), 0u, 0u, 0u);
      auto rigs_metadata       = glm::uvec4(rigs         .size(), 0u, 0u, 0u);

      data.transforms         ->actual()->set_sub_data(0, sizeof transforms_metadata, &transforms_metadata);
      data.materials          ->actual()->set_sub_data(0, sizeof materials_metadata , &materials_metadata );
      data.cameras            ->actual()->set_sub_data(0, sizeof cameras_metadata   , &cameras_metadata   );
      data.lights             ->actual()->set_sub_data(0, sizeof lights_metadata    , &lights_metadata    );
      data.rigs               ->actual()->set_sub_data(0, sizeof rigs_metadata      , &rigs_metadata      );
                             
      data.transforms         ->actual()->set_sub_data(sizeof glm::vec4, sizeof transforms         [0] * transforms_metadata.x     , transforms         .data());
      pbr_materials.size() > 0                                                                     
      ? data.materials        ->actual()->set_sub_data(sizeof glm::vec4, sizeof pbr_materials      [0] * materials_metadata .x     , pbr_materials      .data())
      : data.materials        ->actual()->set_sub_data(sizeof glm::vec4, sizeof phong_materials    [0] * materials_metadata .x     , phong_materials    .data());
      data.cameras            ->actual()->set_sub_data(sizeof glm::vec4, sizeof cameras            [0] * cameras_metadata   .x     , cameras            .data());
      data.lights             ->actual()->set_sub_data(sizeof glm::vec4, sizeof lights             [0] * lights_metadata    .x     , lights             .data());
      data.rigs               ->actual()->set_sub_data(sizeof glm::vec4, sizeof rigs               [0] * rigs_metadata      .x     , rigs               .data());
      data.draw_calls         ->actual()->set_sub_data(0               , sizeof draw_calls         [0] * draw_calls         .size(), draw_calls         .data());
      data.instance_attributes->actual()->set_sub_data(0               , sizeof instance_attributes[0] * instance_attributes.size(), instance_attributes.data());
      
      data.parameter_map->actual()->set("vertex_count", mutable_data.vertex_offset);
      data.parameter_map->actual()->set("draw_count"  , draw_calls.size());

      gl::print_error("Error in Upload Scene Pass");
    });
}
}
}