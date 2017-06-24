#ifndef MAKINA_SYSTEMS_DISPLAY_OUTLETS_WINDOW_HPP_
#define MAKINA_SYSTEMS_DISPLAY_OUTLETS_WINDOW_HPP_

#include <makina/systems/display/outlets/display.hpp>

#include <export.hpp>

namespace mak
{
class MAKINA_EXPORT window : public display
{
public:
  window();
  window(const window&  that) = delete ;
  window(      window&& temp) = default;
  virtual ~window();

  window& operator=(const window&  that) = delete ;
  window& operator=(      window&& temp) = default;

protected:

};
}

#endif
