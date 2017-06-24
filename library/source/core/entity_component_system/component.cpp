#include <makina/core/entity_component_system/component.hpp>

namespace mak
{
component::component()
{
  static std::size_t id = 0;
  id_ = id++;
}
component::component(const component& that) : component()
{
  owner_ = that.owner_;
}

component& component::operator=(const component& that)
{
  owner_ = that.owner_;
  return *this;
}

std::size_t   component::id   () const
{
  return id_;
}
const entity* component::owner() const
{
  return owner_;
}
}
