#include <makina/core/entity_component_system/entity.hpp>

namespace mak
{
entity::entity(const std::string& name) : named(name)
{
  static std::size_t id = 0;
  id_ = id++;
}
entity::entity(const entity& that) : entity()
{
  for(auto& comp : that.components_)
    copy_component(*comp);
}

entity& entity::operator=(const entity& that)
{
  for(auto& comp : that.components_)
    copy_component(*comp);
  return *this;
}

std::size_t  entity::id   () const
{
  return id_;
}
const scene* entity::owner() const
{
  return owner_;
}
}
