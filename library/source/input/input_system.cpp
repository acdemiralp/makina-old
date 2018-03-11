#include <makina/input/input_system.hpp>

namespace mak
{
void input_system::update(frame_timer<>::duration delta)
{
  tick();
}
}
