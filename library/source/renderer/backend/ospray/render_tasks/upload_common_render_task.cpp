#include <makina/renderer/backend/ospray/render_tasks/upload_common_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<upload_common_task_data>* add_upload_common_render_task(renderer* renderer, bool only_raytracing)
{
  return renderer->add_render_task<upload_common_task_data>(
    "Upload Common Data Pass",
    [=] (      upload_common_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const upload_common_task_data& data)
    {
      auto& mutable_data = const_cast<upload_common_task_data&>(data);
      mutable_data.cameras.clear();
      mutable_data.lights .clear();

      auto scene = renderer->scene_cache();

      for (auto& entity : scene->entities<transform, projection>())
      {
        auto metadata   = entity->component<mak::metadata>  ();
        auto transform  = entity->component<mak::transform> ();
        auto projection = entity->component<mak::projection>();
        if (!metadata->active) continue;
        
        auto& camera = mutable_data.cameras.emplace_back(projection->mode() == projection::projection_mode::perspective ? "perspective" : "orthographic");
        
        if      (projection->mode() == projection::projection_mode::perspective)
        {
          
        }
        else if (projection->mode() == projection::projection_mode::orthogonal )
        {
          
        }
        else if (projection->mode() == projection::projection_mode::frustum    )
        {
          
        }
      }


      // All
      camera->set("pos", ospcommon::vec3f());
      camera->set("dir", ospcommon::vec3f());
      camera->set("up", ospcommon::vec3f());
      camera->set("nearClip", 0.1f);
      camera->set("imageStart", ospcommon::vec2f(0.0f, 1.0f));
      camera->set("imageEnd"  , ospcommon::vec2f(1.0f, 0.0f));


      // Perspective
      camera->set("aspect", 0.0f);
      camera->set("fovy", 0.0f);
      camera->set("apertureRadius", 0.0f);
      camera->set("focusDistance", 0.0f);
      camera->set("architectural", false);
      camera->set("stereoMode", 0); // 0 no stereo 1 left 2 right 3 side-by-side
      camera->set("interpupillaryDistance", 67.0f);

      // Orthographic
      camera->set("height", 0.0f);
      camera->set("aspect", 0.0f);

      auto light = ospray::cpp::Light("raytracer", "ambient"); // First parameter depends on the renderer!!!

                                                               // All (+ ambient)
      light.set("color", ospcommon::vec3f());
      light.set("intensity", 1.0f);
      light.set("isVisible", true);

      // Directional
      light.set("direction", ospcommon::vec3f());
      light.set("angularDiameter", 1.0f);

      // Point
      light.set("position", ospcommon::vec3f());
      light.set("radius", 1.0f);

      // Spot
      light.set("position", ospcommon::vec3f());
      light.set("direction", ospcommon::vec3f());
      light.set("openingAngle", 1.0f);
      light.set("penumbraAngle", 1.0f);
      light.set("radius", 1.0f);

      auto lights = std::vector<_light>();
      for (auto& entity : scene->entities<transform, light>())
      {
        auto metadata  = entity->component<mak::metadata> ();
        auto transform = entity->component<mak::transform>();
        auto light     = entity->component<mak::light>    ();
        if (!metadata->active) continue;
        lights.push_back(_light
        {
          glm::uvec4(light->type , 0, 0, 0),
          glm::vec4 (light->color, 0.0f),
          glm::vec4 (light->intensity, light->range, light->spot_angles[0], light->spot_angles[1]),
          glm::vec4 (transform->translation(true), 0.0f),
          glm::vec4 (transform->forward    (true), 0.0f)
        });
      }
    });
}
}
}