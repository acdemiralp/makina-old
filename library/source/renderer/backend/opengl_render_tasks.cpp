#include <makina/renderer/backend/opengl_render_tasks.hpp>

#include <gl/all.hpp>

#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/material.hpp>
#include <makina/resources/mesh.hpp>

namespace mak
{
fg::render_task<clear_task_data>* add_clear_render_task(renderer* framegraph, render_target_resource* render_target, const glm::vec4& color)
{
  return framegraph->add_render_task<clear_task_data>(
    "Clear Pass",
    [&]     (      clear_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.read(render_target);
    },
    [color] (const clear_task_data& data)
    {
      data.target->actual()->clear_color(std::array<float, 4>{color[0], color[1], color[2], color[3]});
    });
}
fg::render_task<phong_task_data>* add_phong_render_task(renderer* framegraph, render_target_resource* render_target)
{
  return framegraph->add_render_task<phong_task_data>(
    "Phong Shading Pass",
    [&]     (      phong_task_data& data, fg::render_task_builder& builder)
    {
      data.target = builder.read(render_target);
    },
    [=]     (const phong_task_data& data)
    {
      auto scene = framegraph->scene_cache();
      
      for (auto& entity : scene->entities<transform, projection> ())
      {
        const auto transform   = entity->component<mak::transform>  ();
        const auto mesh_render = entity->component<mak::projection> ();
      }
      for (auto& entity : scene->entities<transform, light>      ())
      {
        const auto transform   = entity->component<mak::transform>  ();
        const auto light       = entity->component<mak::light>      ();
      }

      std::vector<glm::vec3>     all_vertices           ;
      std::vector<glm::vec3>     all_normals            ;
      std::vector<glm::vec2>     all_texture_coordinates;
      std::vector<glm::uvec3>    all_instance_attributes; // [0] transform index, [1] material index, [2] indices offset
      std::vector<std::uint32_t> all_indices            ;
      for (auto& entity : scene->entities<transform, mesh_render>())
      {
        const auto  transform           = entity->component<mak::transform>  ();
        const auto  mesh_render         = entity->component<mak::mesh_render>();
    
        const auto& vertices            = mesh_render->mesh->vertices           ;
        const auto& normal              = mesh_render->mesh->normals            ;
        const auto& texture_coordinates = mesh_render->mesh->texture_coordinates;
        const auto& indices             = mesh_render->mesh->indices            ;
      }

      // Backend-independent render information:
      // - 5 buffers for vertices, normals, texcoords, instance attributes, indices of all meshes.
      // - 1 buffer  for lights.
      // - 1 buffer  for materials.
      // - 1 buffer  for transform matrices and index spans (per mesh   instance).
      // - 1 buffer  for view and projection matrices       (per camera instance).
      // - N images  for each channel of each material.
      // - 1 index   for the main camera.
      // - 1 retained render target (or 2 for VR) for the backbuffer.

      // Notes:
      // - Any other resources are created and managed by the render tasks.
      // - Render task writing to the backbuffer controls what goes to the screen.

      // Take mesh, material, light, camera data and render onto data.target.
      
    });
}
}