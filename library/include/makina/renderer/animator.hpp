#ifndef MAKINA_RENDERER_ANIMATOR_HPP_
#define MAKINA_RENDERER_ANIMATOR_HPP_

#include <makina/core/frame_timer.hpp>
#include <makina/resources/animation_clip.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT animator
{
  animation_clip*       clip           = nullptr;
  transform*            root_transform = nullptr;
  bool                  play           = true   ;
  frame_timer::duration time           = frame_timer::duration();
};
}

#endif