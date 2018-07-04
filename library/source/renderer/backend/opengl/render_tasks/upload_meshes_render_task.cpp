#include <makina/renderer/backend/opengl/render_tasks/upload_meshes_render_task.hpp>

#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<upload_meshes_task_data>* add_upload_meshes_render_task(renderer* framegraph)
{
  // Shader types with std430 alignment.
  struct glsl_transform
  {
    glm::mat4  model         ;
  };
  struct glsl_phong_material
  {
    glm::uvec4 use_texture   ; // ambient - diffuse - specular - unused
    glm::vec4  ambient       ; // w is unused.
    glm::vec4  diffuse       ; // w is unused.
    glm::vec4  specular      ; // w is shininess.
    glm::uvec4 texture_ids   ; // ambient - diffuse - specular - unused
  };
  struct glsl_physically_based_material
  {
    glm::uvec4 use_texture   ; // albedo - metallicity - roughness - normal
    glm::uvec4 use_texture_2 ; // ambient occlusion - unused - unused - unused
    glm::vec4  albedo        ; // w is unused.
    glm::vec4  properties    ; // metallicity - roughness - unused - unused
    glm::uvec4 textures_ids  ; // albedo - metallicity - roughness - normal
    glm::uvec4 textures_ids_2; // ambient occlusion - unused - unused - unused
  };
  struct glsl_rig
  {
    glm::mat4  model         ;
    glm::mat4  offset        ;
  };

  const glm::uvec3 texture_size {2048, 2048, 32};
  
  const auto retained_vertices            = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Vertices"           , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_normals             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Normals"            , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_colors              = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Colors"             , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_texture_coordinates = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Texture Coordinates", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_bone_ids            = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Bone Ids"           , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_bone_weights        = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Bone Weights"       , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_instance_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Instance Attributes", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_indices             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Indices"            , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_draw_calls          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Draw Calls"         , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_transforms          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Transforms"         , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_materials           = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Materials"          , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_rigs                = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Mesh Rigs"               , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_texture_array       = framegraph->add_retained_resource<texture_description, gl::texture_2d_array>("Mesh Texture Array"      , texture_description{{static_cast<int>(texture_size[0]), static_cast<int>(texture_size[1]), static_cast<int>(texture_size[2])}, GL_RGBA8});
  const auto retained_parameter_map       = framegraph->add_retained_resource<parameter_map::description, parameter_map>("Mesh Parameter Map"      , parameter_map::description());

  return framegraph->add_render_task<upload_meshes_task_data>(
    "Upload Meshes Pass",
    [=] (      upload_meshes_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.write<buffer_resource>          (retained_vertices           );
      data.normals             = builder.write<buffer_resource>          (retained_normals            );
      data.colors              = builder.write<buffer_resource>          (retained_colors             );
      data.texture_coordinates = builder.write<buffer_resource>          (retained_texture_coordinates);
      data.bone_ids            = builder.write<buffer_resource>          (retained_bone_ids           );
      data.bone_weights        = builder.write<buffer_resource>          (retained_bone_weights       );
      data.instance_attributes = builder.write<buffer_resource>          (retained_instance_attributes);
      data.indices             = builder.write<buffer_resource>          (retained_indices            );
      data.draw_calls          = builder.write<buffer_resource>          (retained_draw_calls         );
      data.transforms          = builder.write<buffer_resource>          (retained_transforms         );
      data.materials           = builder.write<buffer_resource>          (retained_materials          );
      data.rigs                = builder.write<buffer_resource>          (retained_rigs               );
      data.textures            = builder.write<texture_2d_array_resource>(retained_texture_array      );
      data.parameter_map       = builder.write<parameter_map_resource>   (retained_parameter_map      );
    },
    [=] (const upload_meshes_task_data& data)
    {
      auto& mutable_data         = const_cast<upload_meshes_task_data&>(data);
                                
      auto  scene                = framegraph->scene_cache();
      auto  mesh_render_entities = scene->entities<transform, mesh_render>        ();
      auto  instance_attributes  = std::vector<glm::uvec2>                        ();
      auto  draw_calls           = std::vector<gl::draw_elements_indirect_command>();
      auto  transforms           = std::vector<glsl_transform>                    ();
      auto  pbr_materials        = std::vector<glsl_physically_based_material>    ();
      auto  phong_materials      = std::vector<glsl_phong_material>               ();
      auto  rigs                 = std::vector<glsl_rig>                          ();
      
      for (auto i = 0; i < mesh_render_entities.size(); ++i)
      {
        const auto& entity      = mesh_render_entities[i];
        const auto  metadata    = entity->component<mak::metadata>   ();
        const auto  transform   = entity->component<mak::transform>  ();
        const auto  mesh_render = entity->component<mak::mesh_render>();
        const auto  animator    = entity->component<mak::animator>   ();
        
        if (!metadata->active) continue;

        transforms.push_back(glsl_transform {transform->matrix(true)});

        auto texture_coordinate_scale = glm::vec3(1.0f);
        auto pbr_material             = dynamic_cast<mak::physically_based_material*>(mesh_render->material);
        if  (pbr_material)
        {
          std::optional<std::uint32_t> albedo_offset, metallicity_offset, roughness_offset, normal_offset, ambient_occlusion_offset;
          if (pbr_material->albedo_image)
          {
            if (!mutable_data.image_cache.count(pbr_material->albedo_image.get()))
            {
              data.textures->actual()->set_sub_image(0, 0, 0, 
                mutable_data.image_offset, 
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
              data.textures->actual()->set_sub_image(0, 0, 0, 
                mutable_data.image_offset, 
                pbr_material->metallicity_image->dimensions()[0], 
                pbr_material->metallicity_image->dimensions()[1], 
                1, 
                GL_RED , 
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
              data.textures->actual()->set_sub_image(0, 0, 0, 
                mutable_data.image_offset, 
                pbr_material->roughness_image->dimensions()[0], 
                pbr_material->roughness_image->dimensions()[1], 
                1, 
                GL_RED , 
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
              data.textures->actual()->set_sub_image(0, 0, 0, 
                mutable_data.image_offset, 
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
              data.textures->actual()->set_sub_image(0, 0, 0, 
                mutable_data.image_offset, 
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

          pbr_materials.push_back(glsl_physically_based_material {
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

          phong_materials.push_back(glsl_phong_material {
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

        if (!mutable_data.mesh_cache.count(mesh_render->mesh))
        {
          const auto& vertices            = mesh_render->mesh->vertices           ;
          const auto& normals             = mesh_render->mesh->normals            ;
          const auto& colors              = mesh_render->mesh->colors             ;
          const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
          const auto& bone_ids            = mesh_render->mesh->bone_ids           ;
          const auto& bone_weights        = mesh_render->mesh->bone_weights       ;
          const auto& indices             = mesh_render->mesh->indices            ;
          
          std::vector<glm::vec4>  transformed_vertices           (vertices           .size());
          std::vector<glm::vec4>  transformed_normals            (normals            .size());
          std::vector<glm::vec3>  transformed_texture_coordinates(texture_coordinates.size());
          std::vector<glm::uvec4> transformed_bone_ids           (bone_ids           .size());
          std::transform(vertices           .begin(), vertices           .end  (), transformed_vertices           .begin(), [ ]                        (const glm::vec3&  vertex)             { return glm::vec4(vertex, 1.0f); });
          std::transform(normals            .begin(), normals            .end  (), transformed_normals            .begin(), [ ]                        (const glm::vec3&  normal)             { return glm::vec4(normal, 0.0f); });
          std::transform(texture_coordinates.begin(), texture_coordinates.end  (), transformed_texture_coordinates.begin(), [texture_coordinate_scale] (const glm::vec3&  texture_coordinate) { return texture_coordinate * texture_coordinate_scale; });
          std::transform(bone_ids           .begin(), bone_ids           .end  (), transformed_bone_ids           .begin(), [mutable_data]             (const glm::uvec4& bone_id)            { return bone_id + glm::uvec4(mutable_data.rig_offset); });

          data.vertices           ->actual()->set_sub_data(sizeof transformed_vertices           [0] * mutable_data.vertex_offset, sizeof transformed_vertices           [0] * transformed_vertices           .size(), transformed_vertices           .data());
          data.normals            ->actual()->set_sub_data(sizeof transformed_normals            [0] * mutable_data.vertex_offset, sizeof transformed_normals            [0] * transformed_normals            .size(), transformed_normals            .data());
          data.colors             ->actual()->set_sub_data(sizeof colors                         [0] * mutable_data.vertex_offset, sizeof colors                         [0] * colors                         .size(), colors                         .data());
          data.texture_coordinates->actual()->set_sub_data(sizeof transformed_texture_coordinates[0] * mutable_data.vertex_offset, sizeof transformed_texture_coordinates[0] * transformed_texture_coordinates.size(), transformed_texture_coordinates.data());
          data.bone_ids           ->actual()->set_sub_data(sizeof transformed_bone_ids           [0] * mutable_data.vertex_offset, sizeof transformed_bone_ids           [0] * transformed_bone_ids           .size(), transformed_bone_ids           .data());
          data.bone_weights       ->actual()->set_sub_data(sizeof bone_weights                   [0] * mutable_data.vertex_offset, sizeof bone_weights                   [0] * bone_weights                   .size(), bone_weights                   .data());
          data.indices            ->actual()->set_sub_data(sizeof indices                        [0] * mutable_data.index_offset , sizeof indices                        [0] * indices                        .size(), indices                        .data());

          mutable_data.mesh_cache[mesh_render->mesh] = gl::draw_elements_indirect_command { static_cast<GLuint>(indices.size()), 1, mutable_data.index_offset, mutable_data.vertex_offset, 0 };
          
          mutable_data.vertex_offset += static_cast<GLuint>(vertices.size());
          mutable_data.index_offset  += static_cast<GLuint>(indices .size());
        }

        auto draw_call          = mutable_data.mesh_cache[mesh_render->mesh];
        draw_call.base_instance = i;
        instance_attributes.push_back({i, i});
        draw_calls         .push_back(draw_call);

        if (animator)
        {
          if (!mutable_data.rig_cache.count(animator))
          {
            auto& transform_bone_pairs = mutable_data.rig_cache[animator];
            std::function<void(mak::transform*)> recursive_collect = [&] (mak::transform* current)
            {
              if (current->metadata()->entity->has_components<mak::bone>())
                transform_bone_pairs.push_back({current, current->metadata()->entity->component<mak::bone>()});
              for (auto child : current->children())
                recursive_collect(child);
            };
            recursive_collect(animator->root_transform);

            std::sort(transform_bone_pairs.begin(), transform_bone_pairs.end(), 
              [ ] (const std::pair<mak::transform*, mak::bone*>& lhs, const std::pair<mak::transform*, mak::bone*>& rhs)
              {
                return lhs.second->index < rhs.second->index;
              });

            mutable_data.rig_offset += static_cast<GLuint>(transform_bone_pairs.size());
          }
          for (auto& transform_bone_pair : mutable_data.rig_cache[animator])
            rigs.push_back(glsl_rig{transform_bone_pair.first->matrix(true), transform_bone_pair.second->offset_matrix});
        }
      }

      auto transforms_metadata = glm::uvec4(transforms   .size(),                                                     0u, 0u, 0u);
      auto materials_metadata  = glm::uvec4(pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size(), 0u, 0u, 0u);
      auto rigs_metadata       = glm::uvec4(rigs         .size(),                                                     0u, 0u, 0u);
      data.instance_attributes->actual()->set_sub_data(0                         , sizeof instance_attributes[0] * instance_attributes.size(), instance_attributes .data());
      data.draw_calls         ->actual()->set_sub_data(0                         , sizeof draw_calls         [0] * draw_calls         .size(), draw_calls          .data());
      data.transforms         ->actual()->set_sub_data(0                         , sizeof transforms_metadata                                , &transforms_metadata       );
      data.transforms         ->actual()->set_sub_data(sizeof transforms_metadata, sizeof transforms         [0] * transforms         .size(), transforms          .data());
      data.materials          ->actual()->set_sub_data(0                         , sizeof materials_metadata                                 , &materials_metadata        );
      pbr_materials.size() > 0                                                                                                                                     
      ? data.materials        ->actual()->set_sub_data(sizeof materials_metadata , sizeof pbr_materials      [0] * pbr_materials      .size(), pbr_materials       .data())
      : data.materials        ->actual()->set_sub_data(sizeof materials_metadata , sizeof phong_materials    [0] * phong_materials    .size(), phong_materials     .data());
      data.rigs               ->actual()->set_sub_data(0                         , sizeof rigs_metadata                                      , &rigs_metadata             );
      data.rigs               ->actual()->set_sub_data(sizeof rigs_metadata      , sizeof rigs               [0] * rigs               .size(), rigs                .data());

      data.parameter_map->actual()->set("vertex_count"   , mutable_data.vertex_offset);
      data.parameter_map->actual()->set("index_count"    , mutable_data.index_offset );
      data.parameter_map->actual()->set("draw_count"     , draw_calls   .size());
      data.parameter_map->actual()->set("transform_count", transforms   .size());
      data.parameter_map->actual()->set("material_count" , pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size());
      data.parameter_map->actual()->set("rig_count"      , rigs         .size());

      gl::print_error("Error in Upload Meshes Pass");
    });
}
}
}