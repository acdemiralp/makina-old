#ifndef MAKINA_SYSTEMS_DISPLAY_OUTLETS_DISPLAY_HPP_
#define MAKINA_SYSTEMS_DISPLAY_OUTLETS_DISPLAY_HPP_

#include <cstddef>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT display
{
public:
  display();
  display(const display&  that) = delete ;
  display(      display&& temp) = default;
  virtual ~display();

  display& operator=(const display&  that) = delete ;
  display& operator=(      display&& temp) = default;

protected:
  std::size_t id_;
};
}

#endif
