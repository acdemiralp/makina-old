#ifndef MAKINA_RESOURCES_AUDIO_CLIP_HPP_
#define MAKINA_RESOURCES_AUDIO_CLIP_HPP_

#include <string>
#include <utility>

#include <ra/resource.hpp>

#include <makina/audio/fmod_context.hpp>
#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
class audio_source;

class MAKINA_EXPORT audio_clip : public named, public ra::resource<audio_clip>
{
public:
  struct MAKINA_EXPORT description
  {
    std::string filepath   ;
    bool        support_3d = true;
  };

  explicit audio_clip  (const description& description);
  audio_clip           (const audio_clip&  that) = delete;
  audio_clip           (      audio_clip&& temp) noexcept;
  virtual ~audio_clip  ();
  audio_clip& operator=(const audio_clip&  that) = delete;
  audio_clip& operator=(      audio_clip&& temp) noexcept;

protected:
  friend audio_source;

  FMOD::Sound* native_;
};
}

#endif