#ifndef MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_RENDER_TASKS_HPP_

#include <makina/renderer/backend/opengl/render_tasks/blit_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/clear_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/composite_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/create_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/immediate_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/phong_line_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/phong_point_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/phong_triangle_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/physically_based_line_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/physically_based_point_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/physically_based_triangle_shading_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/skeletal_animation_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/submit_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/test_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/ui_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/upload_common_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/upload_lines_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/upload_meshes_render_task.hpp>
#include <makina/renderer/backend/opengl/render_tasks/upload_points_render_task.hpp>
#include <makina/export.hpp>

namespace di
{
class opengl_window;
}

namespace mak
{
class engine;

namespace opengl
{
MAKINA_EXPORT void make_default_framegraph(engine* engine, di::opengl_window* main_window);
}
}

#endif