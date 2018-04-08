#include <makina/renderer/backend/vulkan/render_tasks.hpp>

#include <makina/renderer/backend/glsl/shader_sources.hpp>
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
fg::render_task<test_task_data>*                     add_test_render_task                    (renderer* framegraph)
{
  struct vertex_t
  {
    glm::vec3 vertex;
  };
  std::vector<vertex_t> vertices {{{-1.0f, 1.0f, 0.0f}},{{1.0f, 1.0f, 0.0f}},{{1.0f, -1.0f, 0.0f}},{{-1.0f, 1.0f, 0.0f}},{{-1.0f, -1.0f, 0.0f}}};
  const std::uint32_t   draw_count = 5;
 
  return framegraph->add_render_task<test_task_data>(
    "Test Pass",
    [&] (      test_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices = builder.create<buffer_resource>("Test Vertices", buffer_description{
        vertices.size() * sizeof(vertex_t), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer});
 
      data.pipeline = builder.create<pipeline_resource>("Test Pipeline", pipeline_description{
        glsl::test_vertex_shader  , 
        glsl::test_fragment_shader,
        vk::PrimitiveTopology::eTriangleStrip,
        vulkan::context().window_swapchains[0].render_pass, // Test task applies to the first window.
        {
          vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(vertex_t, vertex))
        },
        sizeof vertex_t});
    },
    [=] (const test_task_data& data)
    {
      auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
      for(auto& window_swapchain : vulkan::context().window_swapchains)
      {
        command_buffer->begin           ();
        (*data.vertices->actual         ())->update<vertex_t>(0, {std::uint32_t(vertices.size()), vertices.data()}, command_buffer);
        command_buffer->beginRenderPass (
          window_swapchain.render_pass, 
          window_swapchain.swapchain->getFramebuffer(), 
          vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()), 
          {
            vk::ClearValue(std::array<float, 4>{0.1f, 0.1f, 0.1f}), 
            vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))
          }, 
          vk::SubpassContents::eInline);
        command_buffer->bindPipeline    (vk::PipelineBindPoint::eGraphics, *data.pipeline->actual());
        command_buffer->bindVertexBuffer(0, *data.vertices->actual(), 0);
        command_buffer->setViewport     (0, vk::Viewport(
          0.0f, 
          0.0f, 
          static_cast<float>(window_swapchain.swapchain->getExtent().width ), 
          static_cast<float>(window_swapchain.swapchain->getExtent().height), 
          0.0f, 
          1.0f));
        command_buffer->setScissor      (0, vk::Rect2D({0, 0}, window_swapchain.swapchain->getExtent()));
        command_buffer->draw            (draw_count, 1, 0, 0);
        command_buffer->endRenderPass   ();
        command_buffer->end             ();
        vkhlf::submitAndWait(vulkan::context().graphics_queue, command_buffer);
      }
    });
}
fg::render_task<upload_scene_task_data>*             add_upload_scene_render_task            (renderer* framegraph)
{
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
  
  const glm::uvec2 texture_size {2048, 2048};

  const auto retained_vertices      = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Vertices"     , buffer_description{vk::DeviceSize(128e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer  });
  const auto retained_indices       = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Indices"      , buffer_description{vk::DeviceSize( 64e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer   });
  const auto retained_transforms    = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Transforms"   , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_materials     = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Materials"    , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_cameras       = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Cameras"      , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_lights        = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Lights"       , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst                                           });
  const auto retained_draw_calls    = framegraph->add_retained_resource<buffer_description, std::shared_ptr<vkhlf::Buffer>>("Scene Draw Calls"   , buffer_description{vk::DeviceSize( 16e+6), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndirectBuffer});
  const auto retained_parameter_map = framegraph->add_retained_resource<parameter_map::description, parameter_map>         ("Scene Parameter Map", parameter_map::description());
  // Totals to 128 * 1 + 64 * 1 + 16 * 5 = 272 MB of GPU memory for buffers.

  std::vector<image_resource*> retained_images(64);
  for (auto i = 0; i < retained_images.size(); ++i)
    retained_images[i] = framegraph->add_retained_resource<image_description, std::shared_ptr<vkhlf::Image>>("Scene Texture " + boost::lexical_cast<std::string>(i), image_description
      {vk::ImageType::e2D, vk::Extent3D(texture_size[0], texture_size[1], 1), vk::Format::eR8G8B8A8Unorm, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled});
  // Totals to 64 * 16.77 = 1073 MB of GPU memory for textures.

  return framegraph->add_render_task<upload_scene_task_data>(
    "Upload Scene Pass",
    [&] (      upload_scene_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices      = builder.write<buffer_resource>       (retained_vertices     );
      data.indices       = builder.write<buffer_resource>       (retained_indices      );
      data.transforms    = builder.write<buffer_resource>       (retained_transforms   );
      data.materials     = builder.write<buffer_resource>       (retained_materials    );
      data.cameras       = builder.write<buffer_resource>       (retained_cameras      );
      data.lights        = builder.write<buffer_resource>       (retained_lights       );
      data.draw_calls    = builder.write<buffer_resource>       (retained_draw_calls   );
      data.parameter_map = builder.write<parameter_map_resource>(retained_parameter_map);
      data.images.resize(64);
      for (auto i = 0; i < data.images.size(); ++i)
        data.images[i] = builder.write<image_resource>(retained_images[i]);
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
              static_cast<std::uint32_t>(albedo_handle     .has_value()),
              static_cast<std::uint32_t>(metallicity_handle.has_value()),
              static_cast<std::uint32_t>(roughness_handle  .has_value()),
              static_cast<std::uint32_t>(normal_handle     .has_value())
            },
            glm::uvec4
            {
              static_cast<std::uint32_t>(ambient_occlusion_handle.has_value()), 0, 0, 0
            },
            glm::vec4   (pbr_material->albedo, 0.0f),
            glm::vec4   (pbr_material->metallicity, pbr_material->roughness, 0.0f, 0.0f),
            glm::u64vec4(
              albedo_handle      ? albedo_handle     ->id() : 0,
              metallicity_handle ? metallicity_handle->id() : 0,
              roughness_handle   ? roughness_handle  ->id() : 0,
              normal_handle      ? normal_handle     ->id() : 0),
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
        /*
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
        */
        
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

      auto command_buffer = vulkan::context().command_pool->allocateCommandBuffer();
      command_buffer->begin();
      
      (*data.vertices->actual())->update<_vertex>      (0, {static_cast<std::uint32_t>(vertices.size()), vertices.data()}, command_buffer);
      (*data.indices ->actual())->update<std::uint32_t>(0, {static_cast<std::uint32_t>(indices .size()), indices .data()}, command_buffer);
      
      auto transforms_size = static_cast<std::uint32_t>(transforms     .size());
      auto materials_size  = static_cast<std::uint32_t>(pbr_materials  .size() > 0 ? pbr_materials.size() : phong_materials.size());
      auto cameras_size    = static_cast<std::uint32_t>(cameras        .size());
      auto lights_size     = static_cast<std::uint32_t>(lights         .size());
      (*data.transforms->actual())->update<std::uint32_t>(0, {1u, &transforms_size}, command_buffer);
      (*data.materials ->actual())->update<std::uint32_t>(0, {1u, &materials_size }, command_buffer);
      (*data.cameras   ->actual())->update<std::uint32_t>(0, {1u, &cameras_size   }, command_buffer);
      (*data.lights    ->actual())->update<std::uint32_t>(0, {1u, &lights_size    }, command_buffer);

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
fg::render_task<phong_task_data>*                    add_phong_render_task                   (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  return nullptr;
}
fg::render_task<physically_based_shading_task_data>* add_physically_based_shading_render_task(renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data)
{
  return nullptr;
}
fg::render_task<ui_task_data>*                       add_ui_render_task                      (renderer* framegraph, framebuffer_resource* target)
{
  return nullptr;
}
fg::render_task<present_task_data>*                  add_present_render_task                 (renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<present_task_data>(
    "Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {

    },
    [ ] (const present_task_data& data)
    {
      vulkan::context().present_window_swapchains();
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}
