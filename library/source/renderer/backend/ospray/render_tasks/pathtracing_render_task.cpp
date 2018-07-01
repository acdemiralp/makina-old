#include <makina/renderer/backend/ospray/render_tasks/pathtracing_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<pathtracing_task_data>* add_pathtracing_render_task(
  renderer*                       renderer   ,
  const upload_common_task_data*  common_data, 
  const upload_points_task_data*  point_data , 
  const upload_lines_task_data*   line_data  ,
  const upload_meshes_task_data*  mesh_data  , 
  const upload_volumes_task_data* volume_data, 
  framebuffer_resource*           target     ,
  const std::string&              camera_tag ,
  const pathtracing_settings&     settings   )
{
  return renderer->add_render_task<pathtracing_task_data>(
    "Pathtracing Pass",
    [=] (      pathtracing_task_data& data, fg::render_task_builder& builder)
    {
      data.target   = builder.write(target);
      data.renderer = std::make_unique<::ospray::cpp::Renderer>("pathtracer");
      data.model    = std::make_unique<::ospray::cpp::Model>   ();
      data.renderer->set   ("rouletteDepth", std::int32_t(settings.roulette_depth   ));
      data.renderer->set   ("spp"          , std::int32_t(settings.samples_per_pixel));
      data.renderer->set   ("model"        , *data.model);
      data.renderer->commit();
    },
    [=] (const pathtracing_task_data& data)
    {
      auto& mutable_data = const_cast<pathtracing_task_data&>(data);
      for (const auto datum : point_data ->cache)
        if (!mutable_data.geometry_cache.count(datum.second.geometry.get()))
        {
          mutable_data.model->addGeometry(*datum.second.geometry); 
          mutable_data.geometry_cache.insert(datum.second.geometry.get());
        }
      for (const auto datum : line_data  ->cache)
        if (!mutable_data.geometry_cache.count(datum.second.geometry.get())) 
        {
          mutable_data.model->addGeometry(*datum.second.geometry); 
          mutable_data.geometry_cache.insert(datum.second.geometry.get());
        }
      for (const auto datum : mesh_data  ->cache)
        if (!mutable_data.geometry_cache.count(datum.second.geometry.get())) 
        {
          mutable_data.model->addGeometry(*datum.second.geometry); 
          mutable_data.geometry_cache.insert(datum.second.geometry.get());
        }
      for (const auto datum : volume_data->cache)
        if (!mutable_data.volume_cache.count(datum.second.volume.get())) 
        {
          mutable_data.model->addVolume(*datum.second.volume); 
          mutable_data.volume_cache.insert(datum.second.volume.get());
        }
      mutable_data.model->commit();
      
      auto cameras       = renderer->scene_cache()->entities<metadata, projection>();
      auto camera        = cameras[0];
      auto ospray_camera = &common_data->cameras[0];
      if (!camera_tag.empty())
        for (auto i = 0; i < cameras.size(); ++i)
          if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
          {
            camera        = cameras[i];
            ospray_camera = &common_data->cameras[i];
          }
      
      // Conditional clear.
      auto transform  = camera->component<mak::transform> ();
      auto projection = camera->component<mak::projection>();
      if (!mutable_data.transform_cache  || transform ->matrix() != mutable_data.transform_cache ->matrix() ||
          !mutable_data.projection_cache || projection->matrix() != mutable_data.projection_cache->matrix())
        data.target->actual()->clear(OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM);
      mutable_data.transform_cache  = *transform ;
      mutable_data.projection_cache = *projection;

      data.renderer->set   ("camera", *ospray_camera      );
      data.renderer->set   ("lights", *common_data->lights);
      data.renderer->commit();
      data.renderer->renderFrame(*data.target->actual(), OSP_FB_COLOR | OSP_FB_ACCUM);
    });
}
}
}