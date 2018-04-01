#ifndef MAKINA_AUDIO_AUDIO_SYSTEM_HPP_
#define MAKINA_AUDIO_AUDIO_SYSTEM_HPP_

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT audio_system : public system
{
public:
  audio_system           (); 
  audio_system           (const audio_system&  that) = default;
  audio_system           (      audio_system&& temp) = default;
  virtual ~audio_system  ()                          = default;
  audio_system& operator=(const audio_system&  that) = default;
  audio_system& operator=(      audio_system&& temp) = default;

protected:
  void prepare(scene* scene)                              override;
  void update (frame_timer::duration delta, scene* scene) override;
};
}

#endif