#include <makina/renderer/renderer.hpp>

namespace mak
{
const frame_timer::duration& renderer::delta_cache() const
{
  return delta_cache_;
}
const scene*                 renderer::scene_cache() const
{
  return scene_cache_;
}

void renderer::prepare(                             scene* scene)
{
  compile        ();
  export_graphviz("renderer.gv");
}
void renderer::update (frame_timer::duration delta, scene* scene)
{
  delta_cache_ = delta;
  scene_cache_ = scene;
  execute();
}
}
