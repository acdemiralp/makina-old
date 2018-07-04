#ifndef MAKINA_RENDERER_ANIMATION_SYSTEM_HPP
#define MAKINA_RENDERER_ANIMATION_SYSTEM_HPP

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT animation_system : public system
{
protected:
  void prepare(scene* scene)                              override;
  void update (frame_timer::duration delta, scene* scene) override;
};
}

#endif