#include <makina/audio/audio_source.hpp>

#include <makina/core/logger.hpp>

namespace mak
{
audio_clip*          audio_source::clip        () const
{
  return clip_;
}
bool                 audio_source::autoplay    () const
{
  return autoplay_;
}
bool                 audio_source::looping     () const
{
  return looping_;
}
bool                 audio_source::muted       () const
{
  return muted_;
}
bool                 audio_source::paused      () const
{
  return paused_;
}
float                audio_source::pan         () const
{
  return pan_;
}
float                audio_source::pitch       () const
{
  return pitch_;
}
float                audio_source::priority    () const
{
  return priority_;
}
float                audio_source::volume      () const
{
  return volume_;
}
std::array<float, 2> audio_source::range       () const
{
  return range_;
}

void                 audio_source::set_clip    (audio_clip*                 clip    )
{
  clip_ = clip;
}
void                 audio_source::set_autoplay(bool                        autoplay)
{
  autoplay_ = autoplay;
}
void                 audio_source::set_looping (bool                        looping )
{
  looping_ = looping;
  if (native_) native_->setMode(looping_ ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}
void                 audio_source::set_muted   (bool                        muted   )
{
  muted_ = muted;
  if (native_) native_->setMode(muted_);
}
void                 audio_source::set_paused  (bool                        paused  )
{
  paused_ = paused;
  if (native_) native_->setPaused(paused_);
}
void                 audio_source::set_pan     (float                       pan     )
{
  pan_ = pan;
  if (native_) native_->setPan(pan_);
}
void                 audio_source::set_pitch   (float                       pitch   )
{
  pitch_ = pitch;
  if (native_) native_->setPitch(pitch_);
}
void                 audio_source::set_priority(float                       priority)
{
  priority_ = priority;
  if (native_) native_->setPriority(priority_);
}
void                 audio_source::set_volume  (float                       volume  )
{
  volume_   = volume;
  if (native_) native_->setVolume(volume_);
}
void                 audio_source::set_range   (const std::array<float, 2>& range   )
{
  range_ = range;
  if (native_) native_->set3DMinMaxDistance(range_[0], range_[1]);
}

void                 audio_source::play        ()
{
  if (!clip_)
    logger->error("Unable to play: The clip is nullptr.");

  if (native_)
    stop();

  fmod_context()->playSound(clip_->native_, nullptr, true, &native_);
  native_->setMode            (looping_ ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
  native_->setMode            (muted_   );        
  native_->setPan             (pan_     );
  native_->setPitch           (pitch_   );
  native_->setPriority        (priority_);
  native_->setVolume          (volume_  );
  native_->set3DMinMaxDistance(range_[0], range_[1]);
  native_->setPaused          (paused_  );  
}
bool                 audio_source::playing     () const
{
  auto   is_playing = false;
  if (native_) native_->isPlaying(&is_playing);
  return is_playing;
}
void                 audio_source::stop        () const
{
  if (native_) native_->stop();
}

}
