#include <makina/renderer/backend/vulkan/render_tasks/upload_scene_render_task.hpp>

#include <makina/renderer/backend/vulkan/context.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace vulkan
{
void load_image(mak::image* source, std::shared_ptr<vkhlf::Buffer> intermediate, std::shared_ptr<vkhlf::Image> target, std::size_t offset, const bool single_channel = false)
{
  auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
  command_buffer->begin               ();
  intermediate  ->update<std::uint8_t>(0, {static_cast<std::uint32_t>(source->pixels().size), source->pixels().data}, command_buffer);
  command_buffer->copyBufferToImage   (intermediate, target, vk::ImageLayout::eTransferDstOptimal, vk::BufferImageCopy(0, 0, 0, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 0), vk::Offset3D(0, 0, offset), vk::Extent3D(source->dimensions()[0], source->dimensions()[1], 1)));
  command_buffer->end                 ();
  vkhlf::submitAndWait                (vulkan::context().graphics_queue, command_buffer);
}

fg::render_task<upload_scene_task_data>* add_upload_scene_render_task(renderer* framegraph)
{
  // Shader vertex type.
  struct _vertex
  {
    glm::vec3  position           ;
    glm::vec3  normal             ;
    glm::vec3  texture_coordinates;
    glm::uvec2 instance_attributes;
  };

  // Shader types with std430 alignment.
  struct _transform
  {
    glm::mat4    model        ;
  };
  struct _phong_material
  {
    glm::uvec4   use_texture  ; // ambient - diffuse - specular - unused
    glm::vec4    ambient      ; // w is unused.
    glm::vec4    diffuse      ; // w is unused.
    glm::vec4    specular     ; // w is shininess.
    glm::u64vec4 textures     ; // ambient - diffuse - specular - unused
  };
  struct _physically_based_material
  {
    glm::uvec4   use_texture  ; // albedo - metallicity - roughness - normal
    glm::uvec4   use_texture_2; // ambient occlusion - unused - unused - unused
    glm::vec4    albedo       ; // w is unused.
    glm::vec4    properties   ; // metallicity - roughness - unused - unused
    glm::u64vec4 textures     ; // albedo - metallicity - roughness - normal
    glm::u64vec4 textures_2   ; // ambient occlusion - unused - unused - unused
  };
  struct _camera
  {
    glm::mat4    view         ;
    glm::mat4    projection   ;
  };             
  struct _light  
  {              
    glm::uvec4   type         ; // y, z, w are unused.
    glm::vec4    color        ; // w is unused.
    glm::vec4    properties   ; // intensity - range - inner spot - outer spot
    glm::vec4    position     ; // w is unused.
    glm::vec4    direction    ; // w is unused.
  };
  
  const glm::uvec3 texture_size {2048, 2048, 64};

  const auto retained_vertices      = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Vertices"     , buffer_description{vk::DeviceSize(128e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer  });
  const auto retained_indices       = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Indices"      , buffer_description{vk::DeviceSize( 64e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer   });
  const auto retained_transforms    = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Transforms"   , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_materials     = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Materials"    , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_cameras       = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Cameras"      , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_lights        = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Lights"       , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_draw_calls    = framegraph->add_retained_resource<buffer_description , std::shared_ptr<vkhlf::Buffer>> ("Scene Draw Calls"   , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndirectBuffer});
  // Totals to 128 * 1 + 64 * 1 + 16 * 5 = 272 MB of GPU memory for buffers.

  const auto retained_images        = framegraph->add_retained_resource<image_description  , std::shared_ptr<vkhlf::Image>>  ("Scene Images"       , image_description  {vk::ImageType::e3D , vk::Extent3D(texture_size[0], texture_size[1], texture_size[2]), vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled});
  const auto retained_sampler       = framegraph->add_retained_resource<sampler_description, std::shared_ptr<vkhlf::Sampler>>("Scene Sampler"      , sampler_description{vk::Filter::eLinear, vk::Filter::eLinear});
  // Totals to 64 * 16.77 = 1073 MB of GPU memory for textures.

  const auto retained_parameter_map = framegraph->add_retained_resource<parameter_map::description, parameter_map>           ("Scene Parameter Map", parameter_map::description());

  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [&] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      data.intermediates = builder.create<buffer_resource>       ("Scene Intermediates", buffer_description{vk::DeviceSize(32e+6), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent});
      data.vertices      = builder.write <buffer_resource>       (retained_vertices     );
      data.indices       = builder.write <buffer_resource>       (retained_indices      );
      data.transforms    = builder.write <buffer_resource>       (retained_transforms   );
      data.materials     = builder.write <buffer_resource>       (retained_materials    );
      data.cameras       = builder.write <buffer_resource>       (retained_cameras      );
      data.lights        = builder.write <buffer_resource>       (retained_lights       );
      data.draw_calls    = builder.write <buffer_resource>       (retained_draw_calls   );
      data.images        = builder.write <image_resource>        (retained_images       );
      data.sampler       = builder.write <sampler_resource>      (retained_sampler      );
      data.parameter_map = builder.write <parameter_map_resource>(retained_parameter_map);
    },
    [=] (const upload_scene_task_data& data)
    {
      auto scene                = framegraph->scene_cache();
      auto mesh_render_entities = scene->entities<transform, mesh_render>();
      auto light_entities       = scene->entities<transform, light>      ();
      auto camera_entities      = scene->entities<transform, projection> ();
      auto vertices             = std::vector<_vertex>                       ();
      auto indices              = std::vector<std::uint32_t>                 ();
      auto transforms           = std::vector<_transform>                    ();
      auto pbr_materials        = std::vector<_physically_based_material>    ();
      auto phong_materials      = std::vector<_phong_material>               ();
      auto cameras              = std::vector<_camera>                       ();
      auto lights               = std::vector<_light>                        ();
      auto draw_calls           = std::vector<vk::DrawIndexedIndirectCommand>();

      std::uint32_t texture_offset     = 0;
      std::uint32_t first_index_offset = 0;
      std::int32_t  base_vertex_offset = 0;
      for (auto i = 0; i < mesh_render_entities.size(); ++i)
      {
        const auto& entity      = mesh_render_entities[i];
        const auto  transform   = entity->component<mak::transform>  ();
        const auto  mesh_render = entity->component<mak::mesh_render>();

        glm::vec3 texture_coordinate_scale(1.0f);

        auto pbr_material   = dynamic_cast<mak::physically_based_material*>(mesh_render->material);
        if  (pbr_material)
        {
          std::optional<std::uint32_t> albedo_offset, metallicity_offset, roughness_offset, normal_offset, ambient_occlusion_offset;
          if (pbr_material->albedo_image)
          {
            load_image(pbr_material->albedo_image           .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            albedo_offset = texture_offset++;
          }
          if (pbr_material->metallicity_image)
          {
            load_image(pbr_material->metallicity_image      .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset, true);
            metallicity_offset = texture_offset++;
          }
          if (pbr_material->roughness_image)
          {
            load_image(pbr_material->roughness_image        .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset, true);
            roughness_offset = texture_offset++;
          }
          if (pbr_material->normal_image)
          {
            load_image(pbr_material->normal_image           .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            normal_offset = texture_offset++;
          }
          if (pbr_material->ambient_occlusion_image)
          {
            load_image(pbr_material->ambient_occlusion_image.get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            ambient_occlusion_offset = texture_offset++;
          }

          pbr_materials.push_back(_physically_based_material {
            glm::uvec4 
            {
              static_cast<std::uint32_t>(albedo_offset           .has_value()),
              static_cast<std::uint32_t>(metallicity_offset      .has_value()),
              static_cast<std::uint32_t>(roughness_offset        .has_value()),
              static_cast<std::uint32_t>(normal_offset           .has_value())
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
        auto phong_material = dynamic_cast<mak::phong_material*>(mesh_render->material);
        if  (phong_material)
        {
          std::optional<std::uint32_t> ambient_offset, diffuse_offset, specular_offset;
          if (phong_material->ambient_image)
          {
            load_image(phong_material->ambient_image .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            ambient_offset = texture_offset++;
          }
          if (phong_material->diffuse_image)
          {
            load_image(phong_material->diffuse_image .get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            diffuse_offset = texture_offset++;
          }
          if (phong_material->specular_image)
          {
            load_image(phong_material->specular_image.get(), *data.intermediates->actual(), *data.images->actual(), texture_offset);
            specular_offset = texture_offset++;
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
        
        const auto& mesh_vertices            = mesh_render->mesh->vertices           ;
        const auto& mesh_normals             = mesh_render->mesh->normals            ;
        const auto& mesh_texture_coordinates = mesh_render->mesh->texture_coordinates;
        const auto& mesh_indices             = mesh_render->mesh->indices            ;
       
        for(auto j = 0; j < mesh_vertices.size(); ++j)
          vertices.push_back(_vertex{mesh_vertices[j], mesh_normals[j], texture_coordinate_scale * mesh_texture_coordinates[j], {i, i}});
        indices.insert(indices.end(), mesh_indices.begin(), mesh_indices.end());

        draw_calls.push_back(vk::DrawIndexedIndirectCommand
        {
          static_cast<std::uint32_t>(indices.size()),
          1u,                                        
          first_index_offset,                        
          base_vertex_offset,                        
          static_cast<std::uint32_t>(i)              
        });

        first_index_offset += static_cast<std::uint32_t>(indices .size());
        base_vertex_offset += static_cast<std::uint32_t>(vertices.size());
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

      data.parameter_map->actual()->set("draw_count", draw_calls.size());

      auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
      command_buffer->begin();
      
      (*data.vertices->actual())->update<_vertex>      (0, {static_cast<std::uint32_t>(vertices.size()), vertices.data()}, command_buffer);
      (*data.indices ->actual())->update<std::uint32_t>(0, {static_cast<std::uint32_t>(indices .size()), indices .data()}, command_buffer);
      
      glm::uvec4 transforms_metadata {static_cast<std::uint32_t>(transforms   .size()), 0u, 0u, 0u};
      glm::uvec4 materials_metadata  {static_cast<std::uint32_t>(pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size()), 0u, 0u, 0u};
      glm::uvec4 cameras_metadata    {static_cast<std::uint32_t>(cameras      .size()), 0u, 0u, 0u};
      glm::uvec4 lights_metadata     {static_cast<std::uint32_t>(lights       .size()), 0u, 0u, 0u};
      (*data.transforms->actual())->update<glm::uvec4>(0, {1u, &transforms_metadata}, command_buffer);
      (*data.materials ->actual())->update<glm::uvec4>(0, {1u, &materials_metadata }, command_buffer);
      (*data.cameras   ->actual())->update<glm::uvec4>(0, {1u, &cameras_metadata   }, command_buffer);
      (*data.lights    ->actual())->update<glm::uvec4>(0, {1u, &lights_metadata    }, command_buffer);

      (*data.transforms ->actual())->update<_transform>                    (sizeof glm::vec4, {static_cast<std::uint32_t>(transforms     .size()), transforms     .data()}, command_buffer);
      pbr_materials.size() > 0 
      ? (*data.materials->actual())->update<_physically_based_material>    (sizeof glm::vec4, {static_cast<std::uint32_t>(pbr_materials  .size()), pbr_materials  .data()}, command_buffer)
      : (*data.materials->actual())->update<_phong_material>               (sizeof glm::vec4, {static_cast<std::uint32_t>(phong_materials.size()), phong_materials.data()}, command_buffer);
      (*data.cameras    ->actual())->update<_camera>                       (sizeof glm::vec4, {static_cast<std::uint32_t>(cameras        .size()), cameras        .data()}, command_buffer);
      (*data.lights     ->actual())->update<_light>                        (sizeof glm::vec4, {static_cast<std::uint32_t>(lights         .size()), lights         .data()}, command_buffer);
      (*data.draw_calls ->actual())->update<vk::DrawIndexedIndirectCommand>(0               , {static_cast<std::uint32_t>(draw_calls     .size()), draw_calls     .data()}, command_buffer);

      command_buffer->end  ();
      vkhlf::submitAndWait(vulkan::context().graphics_queue, command_buffer);
    });
}
}
}
