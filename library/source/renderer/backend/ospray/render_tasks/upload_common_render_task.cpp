#include <makina/renderer/backend/ospray/render_tasks/upload_common_render_task.hpp>

#include <makina/core/logger.hpp>

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
      mutable_data.lights .reset();

      const auto scene          = renderer->scene_cache();
      const auto raytracer_type = only_raytracing ? "raytracer" : "pathtracer";

      for (auto& entity : scene->entities<transform, projection>())
      {
        auto metadata   = entity->component<mak::metadata>  ();
        auto transform  = entity->component<mak::transform> ();
        auto projection = entity->component<mak::projection>();
        if (!metadata->active) continue;
        
        auto& camera = mutable_data.cameras.emplace_back(projection->mode() == projection::mode::orthographic ? "orthographic" : "perspective");

        const auto position     = transform->translation(true);
        const auto forward      = transform->forward    (true);
        const auto up           = transform->up         (true);
        camera.set("pos"        , ospcommon::vec3f(position.x, position.y, position.z));
        camera.set("dir"        , ospcommon::vec3f(forward .x, forward .y, forward .z));
        camera.set("up"         , ospcommon::vec3f(up      .x, up      .y, up      .z));
        camera.set("imageStart" , ospcommon::vec2f(0.0f, 1.0f));
        camera.set("imageEnd"   , ospcommon::vec2f(1.0f, 0.0f));

        if      (projection->mode() == projection::mode::perspective)
        {
          const auto parameters = std::get<projection::perspective>(projection->parameters());
          camera.set("fovy"     , parameters.fov_y       );
          camera.set("aspect"   , parameters.aspect_ratio);
          camera.set("nearClip" , parameters.z_range[0]  );

          // TODO: Support the missing in the projection class.
          //camera.set("apertureRadius", 0.0f );
          //camera.set("focusDistance" , 0.0f );
          //camera.set("architectural" , false);
        }
        else if (projection->mode() == projection::mode::orthographic )
        {
          const auto parameters = std::get<projection::orthographic>(projection->parameters());
          const auto height     = std::abs(parameters.rectangle.top   - parameters.rectangle.bottom);
          const auto width      = std::abs(parameters.rectangle.right - parameters.rectangle.left  );
          const auto aspect     = width / height;
          camera.set("height"   , height);
          camera.set("aspect"   , aspect);
          camera.set("nearClip" , parameters.z_range[0]);
        }
        else if (projection->mode() == projection::mode::frustum    )
        {
          logger->error("Ospray does not support frustum projection.");

          // TODO: Convert HMD frustum projection to perspective and set stereo mode to true.
          // camera.set("stereoMode"            , 0); // 0 no stereo 1 left 2 right 3 side-by-side
          // camera.set("interpupillaryDistance", 67.0f);
        }
      }

      std::vector<::ospray::cpp::Light> lights;
      for (auto& entity : scene->entities<transform, light>())
      {
        auto metadata  = entity->component<mak::metadata> ();
        auto transform = entity->component<mak::transform>();
        auto light     = entity->component<mak::light>    ();
        if (!metadata->active) continue;

        auto light_type = std::string("ambient");
        if      (light->type == light::type::directional)
          light_type = "directional";
        else if (light->type == light::type::point)
          light_type = "point";
        else if (light->type == light::type::spot)
          light_type = "spot";

        auto ospray_light = ::ospray::cpp::Light(raytracer_type, light_type);
        ospray_light.set("color"    , ospcommon::vec3f(light->color.x, light->color.y, light->color.z));
        ospray_light.set("intensity", light->intensity);
        ospray_light.set("isVisible", true);

        const auto position = transform->translation(true);
        const auto forward  = transform->forward    (true);
        if      (light->type == light::type::directional)
        {
          ospray_light.set("direction"      , ospcommon::vec3f(forward.x, forward.y, forward.z));

          // TODO: Support the missing in the light class.
          // ospray_light.set("angularDiameter", 1.0f);
        }
        else if (light->type == light::type::point)
        {
          ospray_light.set("position"       , ospcommon::vec3f(position.x, position.y, position.z));
          ospray_light.set("radius"         , light->range);
        }
        else if (light->type == light::type::spot)
        {
          ospray_light.set("position"       , ospcommon::vec3f(position.x, position.y, position.z));
          ospray_light.set("direction"      , ospcommon::vec3f(forward .x, forward .y, forward .z));
          ospray_light.set("openingAngle"   , light->spot_angles[0]);
          ospray_light.set("penumbraAngle"  , light->spot_angles[1]);
          ospray_light.set("radius"         , light->range);
        }

        lights.push_back(ospray_light);
      }
      mutable_data.lights = std::make_unique<::ospray::cpp::Data>(lights.size(), OSP_LIGHT, lights.data());
    });
}
}
}