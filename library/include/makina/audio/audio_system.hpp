#ifndef MAKINA_AUDIO_AUDIO_SYSTEM_HPP
#define MAKINA_AUDIO_AUDIO_SYSTEM_HPP

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT audio_system : public system
{
protected:
  void prepare(scene* scene)                              override;
  void update (frame_timer::duration delta, scene* scene) override;
};
}

#endif