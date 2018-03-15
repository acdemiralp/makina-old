#include <makina/renderer/renderer.hpp>

namespace mak
{
const scene* renderer::scene_cache() const
{
  return scene_cache_;
}

void         renderer::prepare    ()
{
  compile();
}
void         renderer::update     (frame_timer::duration delta, scene* scene)
{
  execute();
}
}
