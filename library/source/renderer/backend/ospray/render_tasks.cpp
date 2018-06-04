#include <makina/renderer/backend/ospray/render_tasks.hpp>

#include <memory>

#include <ospray/ospray_cpp.h>

#include <makina/core/engine.hpp>
#include <makina/renderer/backend/opengl/render_tasks/create_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/submit_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/ui/ui_system.hpp>
#include <makina/vr/vr_system.hpp>

namespace mak
{
namespace ospray
{
void make_default_framegraph(engine* engine, di::window* main_window)
{
  auto argc = 0; ospInit(&argc, nullptr);

  const auto renderer                           = engine->get_system<mak::renderer> ();
  const auto vr_system                          = engine->get_system<mak::vr_system>();

  const auto upload_common_render_task          = ospray::add_upload_common_render_task      (renderer);
  const auto upload_points_render_task          = ospray::add_upload_points_render_task      (renderer);
  const auto upload_lines_render_task           = ospray::add_upload_lines_render_task       (renderer);
  const auto upload_triangles_render_task       = ospray::add_upload_triangles_render_task   (renderer);
                       
  const auto window_size       = main_window->size();
  const auto ospray_backbuffer = renderer->add_retained_resource<framebuffer_description, std::unique_ptr<::ospray::cpp::FrameBuffer>>(
    "Ospray Backbuffer", 
    framebuffer_description
    {
      ospcommon::vec2i(window_size[0], window_size[1]),
      OSP_FB_SRGBA,
      OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM
    });
  const auto opengl_backbuffer = renderer->add_retained_resource(
    "OpenGL Backbuffer", 
    opengl::framebuffer::description(), 
    opengl::default_framebuffer(main_window));

  if (vr_system)
  {
    engine->remove_system<ui_system>(); // 2D UIs do not work well in VR.

    const auto hmd                              = vr_system->hmds()[0];
    const auto create_hmd_textures_render_task  = opengl::add_create_hmd_textures_render_task(renderer, hmd);
    const auto hmd_texture_data                 = create_hmd_textures_render_task->data();
                                                
    const auto left_clear_render_task           = ospray::add_clear_render_task              (renderer, hmd_texture_data.left );                                     
    const auto left_raytracing_render_task      = ospray::add_raytracing_render_task         (
      renderer                            ,                                                       
      upload_common_render_task   ->data(),                                                       
      upload_points_render_task   ->data(),                                                       
      upload_lines_render_task    ->data(),                                                       
      upload_triangles_render_task->data(),                                                       
      hmd_texture_data.left               );                                                 
    const auto left_opengl_adapter_render_task  = ospray::add_opengl_adapter_render_task     (
      renderer                            ,                                                  
      hmd_texture_data.left               );                                                 
                                                                                                  
    const auto right_clear_render_task          = ospray::add_clear_render_task              (
      renderer                            ,                                                  
      hmd_texture_data.right              );                                                 
    const auto right_raytracing_render_task     = ospray::add_raytracing_render_task         (
      renderer,                                                                              
      upload_common_render_task   ->data(),                                                  
      upload_points_render_task   ->data(),                                                  
      upload_lines_render_task    ->data(),                                                  
      upload_triangles_render_task->data(),                                                  
      hmd_texture_data.right              );                                                 
    const auto right_opengl_adapter_render_task = ospray::add_opengl_adapter_render_task     (
      renderer                            , 
      hmd_texture_data.right              );

    const auto submit_hmd_textures_render_task  = opengl::add_submit_hmd_textures_render_task(renderer, hmd, hmd_texture_data);
  }
  else
  {   
  }
}
}
}