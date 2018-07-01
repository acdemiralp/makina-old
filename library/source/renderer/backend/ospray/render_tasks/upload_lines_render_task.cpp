#include <makina/renderer/backend/ospray/render_tasks/upload_lines_render_task.hpp>

#include <makina/core/metadata.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/line_segments.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<upload_lines_task_data>* add_upload_lines_render_task(renderer* renderer, bool only_raytracing)
{
  return renderer->add_render_task<upload_lines_task_data>(
    "Ospray Upload Lines Pass",
    [=] (      upload_lines_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const upload_lines_task_data& data)
    {
      auto& mutable_data = const_cast<upload_lines_task_data&>(data);

      const auto scene = renderer->scene_cache();
      for (auto entity : scene->entities<transform, point_render>())
      {
        auto metadata    = entity->component<mak::metadata>   ();
        auto transform   = entity->component<mak::transform>  ();
        auto line_render = entity->component<mak::line_render>();

        if (!metadata->active || mutable_data.cache.count(line_render)) continue;

        std::vector<glm::vec4>     transformed_vertices(line_render->line_segments->vertices.size());
        std::vector<glm::vec4>     transformed_colors  (line_render->line_segments->colors  .size());
        std::vector<std::uint32_t> transformed_indices (line_render->line_segments->indices .size() / 2);
        std::transform(
          line_render->line_segments->vertices.begin(),
          line_render->line_segments->vertices.end  (),
          transformed_vertices.begin(),
          [&] (const glm::vec3&   vertex) { return glm::vec4 {vertex.x, vertex.y, vertex.z, 1.0f}; });
        std::transform(
          line_render->line_segments->colors  .begin(),
          line_render->line_segments->colors  .end  (),
          transformed_colors.begin(),
          [&] (const glm::u8vec4& color ) { return glm::vec4 {float(color.x) / 255.0f, float(color.y) / 255.0f, float(color.z) / 255.0f, 1.0f}; });
        for (auto i = 0; i < transformed_indices.size(); ++i)
          transformed_indices[i] = line_render->line_segments->indices[i * 2];

        const auto geometry = std::make_shared<::ospray::cpp::Geometry>("streamlines");
        const auto vertices = std::make_shared<::ospray::cpp::Data>(transformed_vertices.size(), OSP_FLOAT3A, transformed_vertices.data()); vertices->commit();
        const auto colors   = std::make_shared<::ospray::cpp::Data>(transformed_colors  .size(), OSP_FLOAT4 , transformed_colors  .data()); colors  ->commit();
        const auto indices  = std::make_shared<::ospray::cpp::Data>(transformed_indices .size(), OSP_INT    , transformed_indices .data()); indices ->commit();
        geometry->set("vertex"      , *vertices);
        geometry->set("vertex.color", *colors  );
        geometry->set("index"       , *indices );
        geometry->set("radius"      , line_render->line_segments->radius);

        const auto phong_material = dynamic_cast<mak::phong_material*>           (line_render->material);
        if (phong_material)
        {
          ::ospray::cpp::Material material(only_raytracing ? "raytracer" : "pathtracer", "OBJMaterial");
          material.set   ("Kd", ospcommon::vec3f(phong_material->diffuse .x, phong_material->diffuse .y, phong_material->diffuse .z));
          material.set   ("Ks", ospcommon::vec3f(phong_material->specular.x, phong_material->specular.y, phong_material->specular.z));
          material.set   ("Ns", phong_material->shininess);
          material.commit();
          geometry->setMaterial(material);
        }
        const auto pbr_material   = dynamic_cast<mak::physically_based_material*>(line_render->material);
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

        mutable_data.cache[line_render] = upload_lines_task_data::line_data
        {
          geometry,
          vertices,
          colors  ,
          indices 
        };
      }
    });
}
}
}