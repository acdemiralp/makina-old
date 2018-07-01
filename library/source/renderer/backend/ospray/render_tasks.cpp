#include <makina/renderer/backend/ospray/render_tasks.hpp>

#include <memory>

#include <ospray/ospray_cpp.h>

#include <makina/core/engine.hpp>
#include <makina/renderer/backend/opengl/render_tasks/blit_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/create_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/submit_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/ui_render_task.hpp>
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
  gl::initialize();

  const auto renderer                           = engine->get_system<mak::renderer> ();
  const auto vr_system                          = engine->get_system<mak::vr_system>();

  const auto upload_common_render_task          = ospray::add_upload_common_render_task (renderer);
  const auto upload_points_render_task          = ospray::add_upload_points_render_task (renderer);
  const auto upload_lines_render_task           = ospray::add_upload_lines_render_task  (renderer);
  const auto upload_meshes_render_task          = ospray::add_upload_meshes_render_task (renderer);
  const auto upload_volumes_render_task         = ospray::add_upload_volumes_render_task(renderer);
                       
  const auto backbuffer  = renderer->add_retained_resource(
    "Backbuffer", 
    opengl::framebuffer::description(), 
    opengl::default_framebuffer(main_window));

  if (vr_system)
  {
    engine->remove_system<ui_system>(); // 2D UIs do not work well in VR.

    const auto hmd                              = vr_system->hmds()[0];
    const auto opengl_hmd_textures_task         = opengl::add_create_hmd_textures_render_task(renderer, hmd);
    const auto ospray_hmd_textures_task         = ospray::add_create_hmd_textures_render_task(renderer, hmd);
    const auto opengl_hmd_texture_data          = opengl_hmd_textures_task->data();
    const auto ospray_hmd_texture_data          = ospray_hmd_textures_task->data();
                                                
    const auto left_raytracing_render_task      = ospray::add_raytracing_render_task         (
      renderer                                  ,                                                       
      &upload_common_render_task ->data()       ,                                                       
      &upload_points_render_task ->data()       ,                                                       
      &upload_lines_render_task  ->data()       ,                                                       
      &upload_meshes_render_task ->data()       ,
      &upload_volumes_render_task->data()       ,
      ospray_hmd_texture_data.left              ,
      "hmd_left_camera"                         );
    const auto left_opengl_adapter_render_task  = ospray::add_opengl_adapter_render_task     (
      renderer                                  ,                                                       
      ospray_hmd_texture_data.left              ,
      opengl_hmd_texture_data.left              );
                                                                                               
    const auto right_raytracing_render_task     = ospray::add_raytracing_render_task         (
      renderer                                  ,                                                                              
      &upload_common_render_task ->data()       ,                                                  
      &upload_points_render_task ->data()       ,                                                  
      &upload_lines_render_task  ->data()       ,                                                  
      &upload_meshes_render_task ->data()       ,
      &upload_volumes_render_task->data()       ,
      ospray_hmd_texture_data.right             ,
      "hmd_right_camera"                        );
    const auto right_opengl_adapter_render_task = ospray::add_opengl_adapter_render_task     (
      renderer                                  ,                                                    
      ospray_hmd_texture_data.right             ,
      opengl_hmd_texture_data.right             );
                                                
    const auto blit_render_task                 = opengl::add_blit_render_task               (renderer, opengl_hmd_texture_data.left, backbuffer);
    const auto submit_hmd_textures_render_task  = opengl::add_submit_hmd_textures_render_task(renderer, hmd, opengl_hmd_texture_data);
  }
  else
  {
    const auto ospray_backbuffer = renderer->add_retained_resource<framebuffer::description, framebuffer>(
      "Ospray Backbuffer", 
      framebuffer::description
      {
        ospcommon::vec2i(main_window->size()[0], main_window->size()[1]),
        OSP_FB_SRGBA,
        OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM
      });
    const auto adapter_framebuffer = renderer->add_retained_resource<opengl::framebuffer::description, opengl::framebuffer>(
      "Adapter Framebuffer", 
      opengl::framebuffer::description({{GLint(main_window->size()[0]), GLint(main_window->size()[1])}}));

    main_window->on_resize.connect([=] (const std::array<std::size_t, 2>& size)
    {
      *ospray_backbuffer  ->actual() = 
        framebuffer        ({ospcommon::vec2i(main_window->size()[0], main_window->size()[1]), OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM});
      *adapter_framebuffer->actual() = 
        opengl::framebuffer(opengl::framebuffer::description({{GLint(main_window->size()[0]), GLint(main_window->size()[1])}}));
    });

    const auto raytracing_render_task           = ospray::add_raytracing_render_task         (
      renderer                                  ,                                                  
      &upload_common_render_task ->data()       ,                                                  
      &upload_points_render_task ->data()       ,                                                  
      &upload_lines_render_task  ->data()       ,                                                  
      &upload_meshes_render_task ->data()       ,
      &upload_volumes_render_task->data()       ,
      ospray_backbuffer                         );                                                 
    const auto opengl_adapter_render_task       = ospray::add_opengl_adapter_render_task     (
      renderer                                  , 
      ospray_backbuffer                         ,
      adapter_framebuffer                       );
    const auto blit_render_task                 = opengl::add_blit_render_task               (
      renderer                                  , 
      adapter_framebuffer                       ,
      backbuffer                                );
    const auto ui_render_task                   = opengl::add_ui_render_task                 (
      renderer                                  , 
      backbuffer                                );
  }
}
}
}