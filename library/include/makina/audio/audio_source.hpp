#ifndef MAKINA_AUDIO_AUDIO_SOURCE_HPP_
#define MAKINA_AUDIO_AUDIO_SOURCE_HPP_

#include <makina/resources/audio_clip.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT audio_source
{
  audio_clip* clip     = nullptr;
  bool        muted    = false  ;
  bool        autoplay = true   ;
  bool        loop     = false  ;
  float       priority = 0.5f   ;
  float       volume   = 1.0f   ;
  float       pitch    = 1.0f   ;
  float       pan      = 0.0f   ;
};
}

#endif