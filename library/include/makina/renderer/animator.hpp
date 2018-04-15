#ifndef MAKINA_RENDERER_ANIMATOR_HPP_
#define MAKINA_RENDERER_ANIMATOR_HPP_

#include <makina/resources/animation_clip.hpp>
#include <makina/resources/bone.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT animator
{
  animation_clip*    clip = nullptr;
  std::vector<bone>* rig  = nullptr;
  bool               play = true   ;
  float              time = 0.0f   ;
};
}

#endif