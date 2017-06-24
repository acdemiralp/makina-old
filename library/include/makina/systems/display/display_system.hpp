#ifndef MAKINA_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_
#define MAKINA_SYSTEMS_DISPLAY_DISPLAY_SYSTEM_HPP_

#include <vector>

#include <makina/core/entity_component_system/system.hpp>
#include <makina/systems/display/outlets/hmd.hpp>
#include <makina/systems/display/outlets/window.hpp>

#include <export.hpp>

namespace mak
{
class MAKINA_EXPORT display_system : public system
{
public:
  display_system();
  display_system(const display_system&  that) = delete ;
  display_system(      display_system&& temp) = default;
  virtual ~display_system();

  display_system& operator=(const display_system&  that) = delete ;
  display_system& operator=(      display_system&& temp) = default;
  
  bool initialize()           override;
  void update    (float time) override;
  void terminate ()           override;

protected:
  std::vector<hmd>    hmds_   ;
  std::vector<window> windows_;
};
}

#endif
