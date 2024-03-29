#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_HPP

#include <di/systems/display/window.hpp>

#include <makina/renderer/backend/ospray/render_tasks/clear_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/create_hmd_textures_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/opengl_adapter_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/pathtracing_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/raytracing_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/tone_mapping_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_common_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_lines_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_meshes_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_points_render_task.hpp>
#include <makina/renderer/backend/ospray/render_tasks/upload_volumes_render_task.hpp>
#include <makina/export.hpp>

namespace mak
{
class engine;

namespace ospray
{
MAKINA_EXPORT void make_default_framegraph(engine* engine, di::window* main_window);
}
}

#endif