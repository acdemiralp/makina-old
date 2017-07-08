#ifndef MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_SCENE_HPP
#define MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_SCENE_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include <makina/core/aspects/named.hpp>
#include <makina/core/entity_component_system/entity.hpp>

#include <makina_export.hpp>

namespace mak
{
class engine;

class MAKINA_EXPORT scene : public named
{
public:
  scene(const std::string& name = "");
  scene(const scene&  that);
  scene(      scene&& temp) = default;
  virtual ~scene()          = default;

  scene& operator=(const scene&  that);
  scene& operator=(      scene&& temp) = default;

  std::size_t          id      () const;
  const engine*        owner   () const;
  std::vector<entity*> entities() const;

  template <typename type, typename... args>
  type* add_entity   (args&&...          arguments);
  template <typename type>                     
  type* copy_entity  (const type&        entity   );
  template <typename type>
  type* get_entity   (const std::string& name     ) const;
  template <typename type>
  type* get_entity   (std::size_t        id       ) const;
  template <typename type>
  void  remove_entity(type*              entity   );
  void  remove_entity(const std::string& name     );
  void  remove_entity(std::size_t        id       );

private:
  friend class engine;

  std::size_t   id_    ;
  const engine* owner_ = nullptr;

  std::vector<std::unique_ptr<entity>> entities_;
};

template <typename type, typename ... args>
type* scene::add_entity   (args&&...          arguments)
{
  entities_.emplace_back(new type(arguments...));
  auto entity = entities_.back().get();
  entity->owner_ = this;
  return static_cast<type*>(entity);
}
template <typename type>                     
type* scene::copy_entity  (const type&        entity   )
{
  entities_.emplace_back(new type(entity));
  auto new_entity = entities_.back().get();
  new_entity->owner_ = this;
  return static_cast<type*>(new_entity);
}
template <typename type>                               
type* scene::get_entity   (const std::string& name     ) const
{
  auto it = std::find_if(entities_.begin(), entities_.end(), 
  [&name] (const std::unique_ptr<entity>& iteratee)
  {
    return iteratee->name() == name;
  });
  return it != entities_.end() ? static_cast<type*>(it->get()) : nullptr;
}
template <typename type>                               
type* scene::get_entity   (std::size_t        id       ) const
{
  auto it = std::find_if(entities_.begin(), entities_.end(), 
  [&id] (const std::unique_ptr<entity>& iteratee)
  {
    return iteratee->id() == id;
  });
  return it != entities_.end() ? static_cast<type*>(it->get()) : nullptr;
}
template <typename type>                               
void  scene::remove_entity(type*              entity   )
{
  entities_.erase(std::remove_if(entities_.begin(), entities_.end(), 
  [&entity] (const std::unique_ptr<mak::entity>& iteratee)
  {
    return iteratee.get() == entity;
  }), entities_.end());
}
}

#endif
