#ifndef MAKINA_RESOURCES_AUDIO_CLIP_LOAD_HPP_
#define MAKINA_RESOURCES_AUDIO_CLIP_LOAD_HPP_

#include <optional>
#include <string>

#include <ra/load.hpp>

#include <makina/core/logger.hpp>
#include <makina/resources/audio_clip.hpp>

struct audio_clip_description
{
  std::string filepath;
};

template<>
inline void ra::load(const std::string&            filepath   , std::optional<mak::audio_clip>* audio_clip)
{
  if (!audio_clip)
  {
    mak::logger->error("Failed to load audio clip: Output is nullptr.");
    return;
  }

  audio_clip->emplace(filepath);
}
template<>
inline void ra::load(const audio_clip_description& description, std::optional<mak::audio_clip>* audio_clip)
{
  if (!audio_clip)
  {
    mak::logger->error("Failed to load audio clip: Output is nullptr.");
    return;
  }

  audio_clip->emplace(description.filepath);
}

#endif