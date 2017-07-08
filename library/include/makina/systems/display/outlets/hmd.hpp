#ifndef MAKINA_SYSTEMS_DISPLAY_OUTLETS_HMD_HPP_
#define MAKINA_SYSTEMS_DISPLAY_OUTLETS_HMD_HPP_

#include <makina/systems/display/outlets/display.hpp>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT hmd : public display
{
public:
  hmd();
  hmd(const hmd&  that) = delete ;
  hmd(      hmd&& temp) = default;
  virtual ~hmd();

  hmd& operator=(const hmd&  that) = delete ;
  hmd& operator=(      hmd&& temp) = default;

protected:

};
}

#endif
