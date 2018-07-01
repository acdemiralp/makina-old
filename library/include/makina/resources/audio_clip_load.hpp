#ifndef MAKINA_RESOURCES_AUDIO_CLIP_LOAD_HPP_
#define MAKINA_RESOURCES_AUDIO_CLIP_LOAD_HPP_

#include <optional>
#include <string>

#include <ra/load.hpp>

#include <makina/core/logger.hpp>
#include <makina/resources/audio_clip.hpp>

template<>
inline void ra::load(const mak::audio_clip::description& description, mak::audio_clip* audio_clip)
{
  if (!audio_clip)
  {
    mak::logger->error("Failed to load audio clip: Output is nullptr.");
    return;
  }

  *audio_clip = mak::audio_clip(description);
}
template<>
inline void ra::load(const std::string&                  filepath   , mak::audio_clip* audio_clip)
{
  ra::load<mak::audio_clip::description, mak::audio_clip>({filepath}, audio_clip);
}

#endif