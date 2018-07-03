#ifndef MAKINA_VR_VR_SYSTEM_HPP_
#define MAKINA_VR_VR_SYSTEM_HPP_

#ifdef _WIN32
#include <windows.h>
#endif

#include <map>

#include <di/systems/vr/vr_system.hpp>

#include <makina/core/system.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT vr_system : public system, public di::vr_system
{
public:
  explicit vr_system  (di::tracking_mode tracking_mode = di::tracking_mode::seated);
  vr_system           (const vr_system&  that) = delete ;
  vr_system           (      vr_system&& temp) = default;
  virtual ~vr_system  ()                       = default;
  vr_system& operator=(const vr_system&  that) = delete ;
  vr_system& operator=(      vr_system&& temp) = default;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;
  
  std::map<di::hmd*                    , transform*> hmd_transform_map_                    ;
  std::map<di::vr_controller*          , transform*> controller_transform_map_             ;
  std::map<di::tracking_reference*     , transform*> tracking_reference_transform_map_     ;
  std::map<di::display_redirect*       , transform*> display_redirect_transform_map_       ;
  std::map<di::generic_tracking_device*, transform*> generic_tracking_device_transform_map_;
};
}

#endif