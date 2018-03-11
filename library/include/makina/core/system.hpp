#ifndef MAKINA_CORE_SYSTEM_HPP_
#define MAKINA_CORE_SYSTEM_HPP_

#include <makina/export.hpp>

namespace mak
{
// A system is anything that updates regularly.
class MAKINA_EXPORT system
{
public:
  virtual ~system     () = default;
  virtual void update () = 0      ;
};
}

#endif