#include <makina/renderer/renderer.hpp>

namespace mak
{
const scene* renderer::scene_cache() const
{
  return scene_cache_;
}

void         renderer::prepare    (scene* scene)
{
  compile        ();
  export_graphviz("renderer.gv");
}
void         renderer::update     (frame_timer::duration delta, scene* scene)
{
  scene_cache_ = scene;
  execute();
}
}
