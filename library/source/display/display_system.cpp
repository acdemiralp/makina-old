#include <makina/display/display_system.hpp>

namespace mak
{
void display_system::update(frame_timer::duration delta, scene* scene)
{
  tick();
}
}
