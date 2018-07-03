#ifndef MAKINA_AUDIO_AUDIO_SOURCE_HPP_
#define MAKINA_AUDIO_AUDIO_SOURCE_HPP_

#include <array>

#include <makina/audio/fmod_context.hpp>
#include <makina/resources/audio_clip.hpp>
#include <makina/export.hpp>

namespace mak
{
class audio_system;

class MAKINA_EXPORT audio_source
{
public:
  audio_clip*          clip          () const;
  bool                 autoplay      () const;
  bool                 looping       () const;
  bool                 muted         () const;
  bool                 paused        () const;
  float                pan           () const;
  float                pitch         () const;
  float                priority      () const;
  float                volume        () const;
  std::array<float, 2> range         () const;
  
  void                 set_clip      (audio_clip*                 clip    );
  void                 set_autoplay  (bool                        autoplay);
  void                 set_looping   (bool                        looping );
  void                 set_muted     (bool                        muted   );
  void                 set_paused    (bool                        paused  );
  void                 set_pan       (float                       pan     );
  void                 set_pitch     (float                       pitch   );
  void                 set_priority  (float                       priority);
  void                 set_volume    (float                       volume  );
  void                 set_range     (const std::array<float, 2>& range   );

  void                 play          ();
  bool                 playing       () const;
  void                 stop          () const;

protected:
  friend audio_system;

  audio_clip*          clip_     = nullptr;
  bool                 autoplay_ = true   ;
  bool                 looping_  = false  ;
  bool                 muted_    = false  ;
  bool                 paused_   = false  ;
  float                pan_      = 0.0f   ;
  float                pitch_    = 1.0f   ;
  float                priority_ = 0.5f   ;
  float                volume_   = 1.0f   ;
  std::array<float, 2> range_    = {0.1f, 1000.0f};

  FMOD::Channel*       native_   = nullptr;
};
}

#endif