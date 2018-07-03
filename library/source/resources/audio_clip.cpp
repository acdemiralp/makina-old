#include <makina/resources/audio_clip.hpp>

#include <stdexcept>
#include <utility>

namespace mak
{
audio_clip::audio_clip           (const description& description) : named(description.filepath)
{
  const auto result = fmod_context()->createSound(description.filepath.c_str(), description.support_3d ? FMOD_3D : FMOD_2D, nullptr, &native_);
  if (result != FMOD_OK)
    throw std::runtime_error("Failed to load audio clip: " + description.filepath);
}
audio_clip::audio_clip           (audio_clip&& temp) noexcept: native_(temp.native_)
{
  temp.native_ = nullptr;
}
audio_clip::~audio_clip          ()
{
  if (native_)
    native_->release();
}
audio_clip& audio_clip::operator=(audio_clip&& temp) noexcept
{
  if (this != &temp)
  {
    native_      = temp.native_;
    temp.native_ = nullptr;
  }
  return *this;
}
}
