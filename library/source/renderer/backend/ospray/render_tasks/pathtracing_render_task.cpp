#include <makina/renderer/backend/ospray/render_tasks/pathtracing_render_task.hpp>

namespace mak
{
namespace ospray
{
fg::render_task<pathtracing_task_data>* add_pathtracing_render_task(
  renderer*                       renderer   ,
  const upload_common_task_data&  common_data, 
  const upload_points_task_data&  point_data , 
  const upload_lines_task_data&   line_data  ,
  const upload_meshes_task_data&  mesh_data  , 
  const upload_volumes_task_data& volume_data, 
  framebuffer_resource*           target     ,
  const std::string&              camera_tag )
{
  return renderer->add_render_task<pathtracing_task_data>(
    "Pathtracing Pass",
    [=] (      pathtracing_task_data& data, fg::render_task_builder& builder)
    {
      data.target   = builder.write(target);
      data.renderer = std::make_unique<::ospray::cpp::Renderer>("pathtracer");
      data.model    = std::make_unique<::ospray::cpp::Model>   ();
      data.renderer->set   ("rouletteDepth", 4);
      data.renderer->set   ("spp"          , 4);
      data.renderer->commit();
    },
    [&] (const pathtracing_task_data& data)
    {
      auto& mutable_data = const_cast<pathtracing_task_data&>(data);
      mutable_data.model = std::make_unique<::ospray::cpp::Model>();
      for (const auto datum : point_data .cache) mutable_data.model->addGeometry(*datum.second.geometry);
      for (const auto datum : line_data  .cache) mutable_data.model->addGeometry(*datum.second.geometry);
      for (const auto datum : mesh_data  .cache) mutable_data.model->addGeometry(*datum.second.geometry);
      for (const auto datum : volume_data.cache) mutable_data.model->addVolume  (*datum.second.volume  );
      mutable_data.model->commit();

      auto camera = &common_data.cameras[0];
      if(!camera_tag.empty())
      {
        auto cameras = renderer->scene_cache()->entities<metadata, projection>();
        for (std::uint32_t i = 0; i < cameras.size(); ++i)
          if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
            camera = &common_data.cameras[i];
      }

      data.renderer->set   ("camera", *camera            );
      data.renderer->set   ("lights", *common_data.lights);
      data.renderer->set   ("model" , *data.model        );
      data.renderer->commit();
      data.renderer->renderFrame(*data.target->actual(), OSP_FB_COLOR | OSP_FB_ACCUM);
    });
}
}
}