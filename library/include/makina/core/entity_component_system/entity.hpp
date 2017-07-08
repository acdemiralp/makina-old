#ifndef MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_ENTITY_HPP
#define MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_ENTITY_HPP

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include <makina/core/aspects/named.hpp>
#include <makina/core/entity_component_system/component_mask.hpp>
#include <makina/core/entity_component_system/component.hpp>

#include <makina_export.hpp>

namespace mak
{
class scene;

class MAKINA_EXPORT entity : public named
{
public:
  entity(const std::string& name = "");
  entity(const entity&      that);
  entity(      entity&&     temp) = default;
 ~entity()                        = default;

  entity& operator=(const entity&  that);
  entity& operator=(      entity&& temp) = default;

  std::size_t  id   () const;
  const scene* owner() const;

  template <typename type, typename... args>
  type* add_component   (args&&...   arguments);
  template <typename type>
  type* copy_component  (const type& component);
  template <typename type>
  type* get_component   () const;
  template <typename type>
  void  remove_component();

private:
  friend class scene;

  std::size_t  id_;
  const scene* owner_ = nullptr;

  std::vector<std::unique_ptr<component>> components_;
};

template <typename type, typename ... args>
type* entity::add_component   (args&&...   arguments)
{
  components_.emplace_back(new type(arguments...));
  auto component = components_.back().get();
  component->owner_ = this;
  return static_cast<type*>(component);
}
template <typename type>
type* entity::copy_component  (const type& component)
{
  components_.emplace_back(new type(component));
  auto new_component = components_.back().get();
  new_component->owner_ = this;
  return static_cast<type*>(new_component);
}
template <typename type>
type* entity::get_component   () const
{
  auto it = std::find_if(components_.begin(), components_.end(), 
  [](const std::unique_ptr<component>& iteratee)
  {
    return typeid(type) == typeid(*iteratee.get());
  });
  return it != components_.end() ? static_cast<type*>(it->get()) : nullptr;
}
template <typename type>
void  entity::remove_component()
{
  components_.erase(std::remove_if(components_.begin(), components_.end(), 
  [](const std::unique_ptr<component>& iteratee)
  {
    return typeid(type) == typeid(*iteratee.get());
  }), components_.end());
}
}

#endif
