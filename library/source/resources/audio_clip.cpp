#include <makina/resources/audio_clip.hpp>

namespace mak
{
audio_clip::audio_clip           (const description& description): named(description.filepath)
{
  fmod_context()->createSound(description.filepath.c_str(), description.support_3d ? FMOD_3D : FMOD_2D, nullptr, &native_);
}
audio_clip::audio_clip           (audio_clip&& temp) noexcept: native_(std::move(temp.native_))
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
    native_      = std::move(temp.native_);
    temp.native_ = nullptr;
  }
  return *this;
}
}
