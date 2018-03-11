#include <makina/renderer/renderer.hpp>

namespace mak
{
void renderer::prepare()
{
  compile();
}
void renderer::update (frame_timer::duration delta, scene* scene)
{
  execute();
}
}
