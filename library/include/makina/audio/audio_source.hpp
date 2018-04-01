#ifndef MAKINA_AUDIO_AUDIO_SOURCE_HPP_
#define MAKINA_AUDIO_AUDIO_SOURCE_HPP_

#include <makina/audio/fmod_context.hpp>
#include <makina/resources/audio_clip.hpp>
#include <makina/export.hpp>

namespace mak
{
class audio_system;

class MAKINA_EXPORT audio_source
{
public:
  audio_clip* clip          () const;
  bool        muted         () const;
  bool        autoplay      () const;
  bool        loop          () const;
  float       priority      () const;
  float       volume        () const;
  float       pitch         () const;
  float       pan           () const;
              
  void        set_clip      (audio_clip* clip    );
  void        set_muted     (bool        muted   );
  void        set_autoplay  (bool        autoplay);
  void        set_loop      (bool        loop    );
  void        set_priority  (float       priority);
  void        set_volume    (float       volume  );
  void        set_pitch     (float       pitch   );
  void        set_pan       (float       pan     );

protected:
  friend audio_system;

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