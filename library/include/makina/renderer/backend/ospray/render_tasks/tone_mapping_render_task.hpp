#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_TONE_MAPPING_RENDER_TASK_HPP
#define MAKINA_RENDERER_BACKEND_OSPRAY_RENDER_TASKS_TONE_MAPPING_RENDER_TASK_HPP

#include <fg/render_task.hpp>

#include <makina/renderer/backend/ospray/framebuffer.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT tone_mapping_settings
{
  float contrast   = 1.6773f ;
  float shoulder   = 0.9714f ;
  float mid_in     = 0.18f   ;
  float mid_out    = 0.18f   ;
  float hdr_max    = 11.0785f;
  bool  aces_color = true    ;
};

struct MAKINA_EXPORT tone_mapping_task_data
{
  framebuffer_resource* target;
};

MAKINA_EXPORT fg::render_task<tone_mapping_task_data>* add_tone_mapping_render_task(renderer* renderer, framebuffer_resource* target, tone_mapping_settings settings = tone_mapping_settings());
}
}

#endif