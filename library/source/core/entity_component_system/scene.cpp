#include <makina/core/entity_component_system/scene.hpp>

namespace mak
{
scene::scene(const std::string& name) : named(name)
{
  static std::size_t id = 0;
  id_ = id++;
}
scene::scene(const scene& that) : scene()
{
  owner_ = that.owner_;
  for (auto& entity : that.entities_)
    copy_entity(*entity);
}
  
scene& scene::operator=(const scene& that)
{
  owner_ = that.owner_;
  for (auto& entity : that.entities_)
    copy_entity(*entity);
  return *this;
}

std::size_t          scene::id      () const
{
  return id_;
}
const engine*        scene::owner   () const
{
  return owner_;
}
std::vector<entity*> scene::entities() const
{
  std::vector<entity*> raw_entities(entities_.size());
  std::transform(entities_.begin(), entities_.end(), raw_entities.begin(),
  [](const std::unique_ptr<entity>& iteratee)
  {
    return iteratee.get();
  });
  return raw_entities;
}

void scene::remove_entity(const std::string& name)
{
  entities_.erase(std::remove_if(entities_.begin(), entities_.end(), 
  [&name] (const std::unique_ptr<entity>& iteratee)
  {
    return iteratee->name() == name;
  }), entities_.end());
}
void scene::remove_entity(std::size_t        id  )
{
  entities_.erase(std::remove_if(entities_.begin(), entities_.end(), 
  [&id] (const std::unique_ptr<entity>& iteratee)
  {
    return iteratee->id() == id;
  }), entities_.end());
}
}
