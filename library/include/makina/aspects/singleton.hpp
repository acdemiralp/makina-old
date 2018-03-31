#ifndef MAKINA_ASPECTS_SINGLETON_HPP_
#define MAKINA_ASPECTS_SINGLETON_HPP_

#include <utility>

#include <makina/export.hpp>

namespace mak
{
// Note: Do not use a singleton unless you are able to justify that 
// it is the only optimal solution to the problem you are facing.
template<typename derived>
class MAKINA_EXPORT singleton
{
public:
  static derived& get()
  {
    static derived instance;
    return instance;
  }
};
}

#endif