#ifndef MAKINA_AUDIO_AUDIO_SOURCE_HPP_
#define MAKINA_AUDIO_AUDIO_SOURCE_HPP_

#include <makina/audio/fmod_context.hpp>
#include <makina/resources/audio_clip.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT audio_source
{
public:

protected:
  audio_clip*    clip_     = nullptr;
  bool           muted_    = false  ;
  bool           autoplay_ = true   ;
  bool           loop_     = false  ;
  float          priority_ = 0.5f   ;
  float          volume_   = 1.0f   ;
  float          pitch_    = 1.0f   ;
  float          pan_      = 0.0f   ;

  FMOD::Channel* native_   = nullptr;
};
}

#endif