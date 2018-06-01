#include <makina/renderer/backend/opengl/render_tasks.hpp>

#include <gl/all.hpp>

#include <makina/core/engine.hpp>
#include <makina/input/input_system.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/ui/ui_system.hpp>
#include <makina/vr/vr_system.hpp>

namespace mak
{
void opengl::make_default_framegraph(engine* engine, di::opengl_window* main_window)
{
  gl::initialize();

  const auto input_system                   = engine->get_system<mak::input_system>();
  const auto renderer                       = engine->get_system<mak::renderer>();
  const auto vr_system                      = engine->get_system<mak::vr_system>();

  const auto backbuffer                     = renderer->add_retained_resource("Backbuffer", framebuffer::description(), default_framebuffer(main_window));

  const auto upload_common_render_task      = add_upload_common_render_task     (renderer);
  const auto upload_lines_render_task       = add_upload_lines_render_task      (renderer);
  const auto upload_meshes_render_task      = add_upload_meshes_render_task     (renderer);
  const auto upload_points_render_task      = add_upload_points_render_task     (renderer);
  const auto skeletal_animation_render_task = add_skeletal_animation_render_task(renderer, upload_meshes_render_task->data());
        auto upload_meshes_task_data        = upload_meshes_render_task     ->data();
  const auto skeletal_animation_task_data   = skeletal_animation_render_task->data();
  upload_meshes_task_data.vertices          = skeletal_animation_task_data.transformed_vertices;
  upload_meshes_task_data.normals           = skeletal_animation_task_data.transformed_normals ;

  if (vr_system)
  {
    engine->remove_system<ui_system>(); // 2D UIs do not work well in VR.

    const auto hmd                             = vr_system->hmds()[0];
    const auto create_hmd_textures_render_task = add_create_hmd_textures_render_task              (renderer, hmd);
    const auto hmd_texture_data                = create_hmd_textures_render_task->data();
    const auto left_texture                    = hmd_texture_data.left ;
    const auto right_texture                   = hmd_texture_data.right;
    const auto left_clear_render_task          = add_clear_render_task                            (renderer,               left_texture , {0.1F, 0.1F, 0.1F, 1.0F});
    const auto left_pb_triangle_render_task    = add_physically_based_triangle_shading_render_task(renderer,               left_texture , upload_common_render_task->data(), upload_meshes_task_data, "hmd_left_camera" );
    const auto left_pb_line_render_task        = add_physically_based_line_shading_render_task    (renderer,               left_texture , upload_common_render_task->data(), upload_lines_render_task ->data());
    const auto left_pb_point_render_task       = add_physically_based_point_shading_render_task   (renderer,               left_texture , upload_common_render_task->data(), upload_points_render_task->data());
    const auto left_immediate_render_task      = add_immediate_render_task                        (renderer, input_system, left_texture , upload_common_render_task->data());
    const auto right_clear_render_task         = add_clear_render_task                            (renderer,               right_texture, {0.1F, 0.1F, 0.1F, 1.0F});
    const auto right_pb_triangle_render_task   = add_physically_based_triangle_shading_render_task(renderer,               right_texture, upload_common_render_task->data(), upload_meshes_task_data, "hmd_right_camera");
    const auto right_pb_line_render_task       = add_physically_based_line_shading_render_task    (renderer,               right_texture, upload_common_render_task->data(), upload_lines_render_task ->data());
    const auto right_pb_point_render_task      = add_physically_based_point_shading_render_task   (renderer,               right_texture, upload_common_render_task->data(), upload_points_render_task->data());
    const auto right_immediate_render_task     = add_immediate_render_task                        (renderer, input_system, right_texture, upload_common_render_task->data());
    const auto blit_render_task                = add_blit_render_task                             (renderer,               left_texture , backbuffer);
    const auto submit_hmd_textures_render_task = add_submit_hmd_textures_render_task              (renderer, hmd, hmd_texture_data);
  }
  else
  {                                                                          
    const auto clear_render_task               = add_clear_render_task                            (renderer,               backbuffer, {0.1F, 0.1F, 0.1F, 1.0F});
    const auto pb_triangle_render_task         = add_physically_based_triangle_shading_render_task(renderer,               backbuffer, upload_common_render_task->data(), upload_meshes_task_data);
    const auto pb_line_render_task             = add_physically_based_line_shading_render_task    (renderer,               backbuffer, upload_common_render_task->data(), upload_lines_render_task ->data());
    const auto pb_point_render_task            = add_physically_based_point_shading_render_task   (renderer,               backbuffer, upload_common_render_task->data(), upload_points_render_task->data());
    const auto immediate_render_task           = add_immediate_render_task                        (renderer, input_system, backbuffer, upload_common_render_task->data());
    const auto ui_render_task                  = add_ui_render_task                               (renderer,               backbuffer);
  }
}
}
