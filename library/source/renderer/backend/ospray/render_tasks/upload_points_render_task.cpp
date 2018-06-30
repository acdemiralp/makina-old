#include <makina/renderer/backend/ospray/render_tasks/upload_points_render_task.hpp>

#include <makina/renderer/point_render.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace ospray
{
  fg::render_task<upload_points_task_data>* add_upload_points_render_task(renderer* renderer, bool only_raytracing)
  {
    return renderer->add_render_task<upload_points_task_data>(
      "Ospray Upload Points Pass",
      [=] (      upload_points_task_data& data, fg::render_task_builder& builder)
      {

      },
      [=] (const upload_points_task_data& data)
      {
        auto& mutable_data = const_cast<upload_points_task_data&>(data);

        const auto scene = renderer->scene_cache();
        for (auto entity : scene->entities<transform, point_render>())
        {
          auto transform    = entity->component<mak::transform>   ();
          auto point_render = entity->component<mak::point_render>();

          if (mutable_data.point_cloud_cache.count(point_render->point_cloud)) continue;

          struct sphere
          {
            float x, y, z;
            int   index  ;
          };
          auto index = 0;
          std::vector<sphere> spheres(point_render->point_cloud->vertices.size());
          std::transform(
            point_render->point_cloud->vertices.begin(), 
            point_render->point_cloud->vertices.end  (), 
            spheres.begin(), 
            [&] (const glm::vec3& vertex) { return sphere{vertex.x, vertex.y, vertex.z, index++}; });

          auto geometry = std::make_shared<::ospray::cpp::Geometry>("spheres");
          auto vertices = std::make_shared<::ospray::cpp::Data>(spheres.size() * sizeof sphere          , OSP_CHAR  , spheres.data()                          );
          auto colors   = std::make_shared<::ospray::cpp::Data>(point_render->point_cloud->colors.size(), OSP_UCHAR4, point_render->point_cloud->colors.data());
          geometry->set        ("spheres"         , *vertices.get());
          geometry->set        ("color"           , *colors  .get());
          geometry->set        ("radius"          , point_render->point_cloud->radius);
          geometry->set        ("bytes_per_sphere", std::int32_t(sizeof sphere   ));
          geometry->set        ("offset_colorID"  , std::int32_t(sizeof glm::vec3));
          
          auto phong_material = dynamic_cast<mak::phong_material*>           (point_render->material);
          if  (phong_material)
          {
            ::ospray::cpp::Material material(only_raytracing ? "raytracer" : "pathtracer", "OBJMaterial");
            material.set   ("Kd", ospcommon::vec3f(phong_material->diffuse .x, phong_material->diffuse .y, phong_material->diffuse .z));
            material.set   ("Ks", ospcommon::vec3f(phong_material->specular.x, phong_material->specular.y, phong_material->specular.z));
            material.set   ("Ns", phong_material->shininess);
            material.commit();
            geometry->setMaterial(material);
          }
          auto pbr_material   = dynamic_cast<mak::physically_based_material*>(point_render->material);
          if  (pbr_material)
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

          mutable_data.point_cloud_cache[point_render->point_cloud] = upload_points_task_data::point_data
          {
            std::move(geometry),
            std::move(vertices),
            std::move(colors  )
          };
        }
      });
  }
}
}