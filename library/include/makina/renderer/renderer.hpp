#ifndef MAKINA_RENDERER_RENDERER_HPP_
#define MAKINA_RENDERER_RENDERER_HPP_

#include <fg/framegraph.hpp>

#include <makina/core/system.hpp>
#include <makina/export.hpp>

namespace mak
{
// Renderer is a framegraph system.
class MAKINA_EXPORT renderer : public fg::framegraph, public system
{
public:
  const scene* scene_cache() const;

protected:
  void         prepare    (scene* scene)                              override;
  void         update     (frame_timer::duration delta, scene* scene) override;

  scene* scene_cache_ = nullptr;
};
}

#endif