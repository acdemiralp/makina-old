#ifndef MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_COMPONENT_HPP
#define MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_COMPONENT_HPP

#include <cstddef>

#include <makina_export.hpp>

namespace mak
{
class entity;

class MAKINA_EXPORT component
{
public:
  component();
  component(const component&  that);
  component(      component&& temp) = default;
  virtual ~component()              = default;

  component& operator=(const component&  that);
  component& operator=(      component&& temp) = default;

  std::size_t   id   () const;
  const entity* owner() const;

private:
  friend class entity;

  std::size_t   id_    ;
  const entity* owner_ = nullptr;
};
}

#endif
