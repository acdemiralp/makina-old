#include <makina/audio/audio_source.hpp>

namespace mak
{
audio_clip* audio_source::clip        () const
{
  return clip_;
}
bool        audio_source::muted       () const
{
  return muted_;
}
bool        audio_source::autoplay    () const
{
  return autoplay_;
}
bool        audio_source::loop        () const
{
  return loop_;
}
float       audio_source::priority    () const
{
  return priority_;
}
float       audio_source::volume      () const
{
  return volume_;
}
float       audio_source::pitch       () const
{
  return pitch_;
}
float       audio_source::pan         () const
{
  return pan_;
}
            
void        audio_source::set_clip    (audio_clip* clip    )
{
  clip_ = clip;
}
void        audio_source::set_muted   (bool        muted   )
{
  muted_ = muted;
}
void        audio_source::set_autoplay(bool        autoplay)
{
  autoplay_ = autoplay;
}
void        audio_source::set_loop    (bool        loop    )
{
  loop_ = loop;
}
void        audio_source::set_priority(float       priority)
{
  priority_ = priority;
}
void        audio_source::set_volume  (float       volume  )
{
  volume_ = volume;
}
void        audio_source::set_pitch   (float       pitch   )
{
  pitch_ = pitch;
}
void        audio_source::set_pan     (float       pan     )
{
  pan_ = pan;
}
}