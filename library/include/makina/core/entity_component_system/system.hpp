#ifndef MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_SYSTEM_HPP
#define MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_SYSTEM_HPP

#include <cstddef>

#include <makina_export.hpp>

namespace mak
{
class engine;

class MAKINA_EXPORT system
{
public:
  system();
  system(const system&  that);
  system(      system&& temp) = default;
  virtual ~system()           = default;

  system& operator=(const system&  that);
  system& operator=(      system&& temp) = default;

  std::size_t   id   () const;
  const engine* owner() const;

  virtual bool initialize()           = 0;
  virtual void update    (float time) = 0;
  virtual void terminate ()           = 0;

private:
  friend class engine;

  std::size_t   id_    ;
  const engine* owner_ = nullptr;
};
}

#endif
