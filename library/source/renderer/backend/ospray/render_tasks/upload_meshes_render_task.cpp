#include <makina/renderer/backend/ospray/render_tasks/upload_meshes_render_task.hpp>

#include <makina/core/metadata.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<upload_meshes_task_data>* add_upload_meshes_render_task(renderer* renderer, bool only_raytracing)
{
  auto render_task = renderer->add_render_task<upload_meshes_task_data>(
    "Upload Meshes Pass",
    [=] (      upload_meshes_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const upload_meshes_task_data& data)
    {
      auto& mutable_data = const_cast<upload_meshes_task_data&>(data);

      const auto scene = renderer->scene_cache();
      for (auto entity : scene->entities<transform, mesh_render>())
      {
        auto metadata    = entity->component<mak::metadata>   ();
        auto mesh_render = entity->component<mak::mesh_render>();

        if (!metadata->active || mutable_data.cache.count(mesh_render)) continue;
        
        std::vector<glm::vec3>  transformed_vertices           (mesh_render->mesh->vertices           .size());
        std::vector<glm::vec4>  transformed_colors             (mesh_render->mesh->colors             .size());
        std::vector<glm::vec2>  transformed_texture_coordinates(mesh_render->mesh->texture_coordinates.size());

        std::transform(
          mesh_render->mesh->vertices.begin(),
          mesh_render->mesh->vertices.end  (),
          transformed_vertices       .begin(),
          [ ] (const glm::vec3& vertex) { return glm::vec3 {vertex.x, vertex.y, -vertex.z}; });
        std::transform(
          mesh_render->mesh->colors.begin(),
          mesh_render->mesh->colors.end  (),
          transformed_colors       .begin(),
          [ ] (const glm::u8vec4& color ) { return glm::vec4 {float(color.x) / 255.0f, float(color.y) / 255.0f, float(color.z) / 255.0f, 1.0f}; });
        std::transform(
          mesh_render->mesh->texture_coordinates.begin(),
          mesh_render->mesh->texture_coordinates.end  (),
          transformed_texture_coordinates       .begin(),
          [ ] (const glm::vec3& texture_coordinate) { return glm::vec2 {texture_coordinate.x, texture_coordinate.y}; });

        const auto geometry            = std::make_shared<::ospray::cpp::Geometry>("triangles");
        const auto vertices            = std::make_shared<::ospray::cpp::Data>(transformed_vertices           .size()    , OSP_FLOAT3, transformed_vertices           .data()); vertices           ->commit();
        const auto colors              = std::make_shared<::ospray::cpp::Data>(transformed_colors             .size()    , OSP_FLOAT4, transformed_colors             .data()); colors             ->commit();
        const auto texture_coordinates = std::make_shared<::ospray::cpp::Data>(transformed_texture_coordinates.size()    , OSP_FLOAT2, transformed_texture_coordinates.data()); texture_coordinates->commit();
        const auto indices             = std::make_shared<::ospray::cpp::Data>(mesh_render->mesh->indices     .size() / 3, OSP_INT3  , mesh_render->mesh->indices     .data()); indices            ->commit();
        if (mesh_render->mesh->vertices           .size()) geometry->set("vertex"         , *vertices           );
        if (mesh_render->mesh->colors             .size()) geometry->set("vertex.color"   , *colors             );
        if (mesh_render->mesh->texture_coordinates.size()) geometry->set("vertex.texcoord", *texture_coordinates);
        if (mesh_render->mesh->indices            .size()) geometry->set("index"          , *indices            );

        const auto phong_material = dynamic_cast<mak::phong_material*>           (mesh_render->material);
        if (phong_material)
        {
          ::ospray::cpp::Material material(only_raytracing ? "raytracer" : "pathtracer", "OBJMaterial");
          material.set   ("Kd", ospcommon::vec3f(phong_material->diffuse .x, phong_material->diffuse .y, phong_material->diffuse .z));
          material.set   ("Ks", ospcommon::vec3f(phong_material->specular.x, phong_material->specular.y, phong_material->specular.z));
          material.set   ("Ns", phong_material->shininess);
          material.commit();
          geometry->setMaterial(material);
        }
        const auto pbr_material   = dynamic_cast<mak::physically_based_material*>(mesh_render->material);
        if (pbr_material)
        {
          if (only_raytracing)
          {
            ::ospray::cpp::Material material("raytracer", "OBJMaterial");
            material.set   ("Kd", ospcommon::vec3f(pbr_material->albedo.x, pbr_material->albedo.y, pbr_material->albedo.z));
            material.set   ("Ks", ospcommon::vec3f(pbr_material->albedo.x, pbr_material->albedo.y, pbr_material->albedo.z));
            material.set   ("Ns", pbr_material->roughness);
            material.commit();
            geometry->setMaterial(material);
          }
          else
          {
            ::ospray::cpp::Material material("pathtracer", "principled");
            material.set   ("baseColor", ospcommon::vec3f(pbr_material->albedo.x, pbr_material->albedo.y, pbr_material->albedo.z));
            material.set   ("metallic" , pbr_material->metallicity);
            material.set   ("roughness", pbr_material->roughness  );
            material.commit();
            geometry->setMaterial(material);
          }
        }

        geometry->commit();

        mutable_data.cache[mesh_render] = upload_meshes_task_data::mesh_data
        {
          geometry           ,
          vertices           ,
          colors             ,
          texture_coordinates,
          indices 
        };
      }
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}