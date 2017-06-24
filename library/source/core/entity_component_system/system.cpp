#include <makina/core/entity_component_system/system.hpp>

namespace mak
{
system::system()
{
  static std::size_t id = 0;
  id_ = id++;
}
system::system(const system& that) : system()
{
  owner_ = that.owner_;
}
  
system& system::operator=(const system& that)
{
  owner_ = that.owner_;
  return *this;
}

std::size_t   system::id   () const
{
  return id_;
}
const engine* system::owner() const
{
  return owner_;
}
}
