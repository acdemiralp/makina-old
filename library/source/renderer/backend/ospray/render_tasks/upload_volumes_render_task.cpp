#include <makina/renderer/backend/ospray/render_tasks/upload_volumes_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<upload_volumes_task_data>* add_upload_volumes_render_task(renderer* renderer, bool only_raytracing)
{
  return renderer->add_render_task<upload_volumes_task_data>(
    "Ospray Upload Volumes Pass",
    [=] (      upload_volumes_task_data& data, fg::render_task_builder& builder)
    {

    },
    [=] (const upload_volumes_task_data& data)
    {
      auto& mutable_data = const_cast<upload_volumes_task_data&>(data);

      const auto scene = renderer->scene_cache();
      for (auto entity : scene->entities<transform, point_render>())
      {
        auto transform     = entity->component<mak::transform>    ();
        auto volume_render = entity->component<mak::volume_render>();

        if (mutable_data.volume_cache.count(volume_render->volume)) continue;

        const auto volume            = std::make_shared<::ospray::cpp::Volume>          ("shared_structured_volume");
        const auto voxel_data        = std::make_shared<::ospray::cpp::Data>            (volume_render->volume->data.num_elements(), OSP_FLOAT, volume_render->volume->data.data()); voxel_data->commit();
        const auto transfer_function = std::make_shared<::ospray::cpp::TransferFunction>("piecewise_linear");
        const auto colors            = std::make_shared<::ospray::cpp::Data>            (volume_render->transfer_function->colors   .size(), OSP_FLOAT3, volume_render->transfer_function->colors   .data()); colors  ->commit();
        const auto opacities         = std::make_shared<::ospray::cpp::Data>            (volume_render->transfer_function->opacities.size(), OSP_FLOAT , volume_render->transfer_function->opacities.data()); opacities->commit();
        transfer_function->set   ("colors"          , *colors);
        transfer_function->set   ("opacities"       , *opacities);
        transfer_function->set   ("valueRange"      , ospcommon::vec2f{0.0f, 1.0f});
        transfer_function->commit();
        volume           ->set   ("dimensions"      , ospcommon::vec3i(volume_render->volume->data.shape()[0], volume_render->volume->data.shape()[1], volume_render->volume->data.shape()[2]));
        volume           ->set   ("gridSpacing"     , ospcommon::vec3f(volume_render->volume->spacing     [0], volume_render->volume->spacing     [1], volume_render->volume->spacing     [2]));
        volume           ->set   ("transferFunction", *transfer_function);
        volume           ->set   ("voxelData"       , *voxel_data);
        volume           ->set   ("voxelType"       , "float");
        volume           ->commit();
        
        mutable_data.volume_cache[volume_render->volume] = upload_volumes_task_data::volume_data
        {
          volume           ,
          voxel_data       ,
          transfer_function,
          colors           ,
          opacities
        };
      }
    });
}
}
}