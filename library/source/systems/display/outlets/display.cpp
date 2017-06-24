#include <makina/systems/display/outlets/display.hpp>

namespace mak
{
display::display()
{
  static std::size_t id = 0;
  id_ = id++;
}
display::~display()
{

}
}
