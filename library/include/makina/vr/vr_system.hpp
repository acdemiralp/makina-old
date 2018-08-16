#ifndef MAKINA_VR_VR_SYSTEM_HPP
#define MAKINA_VR_VR_SYSTEM_HPP

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
  
  entity*                                                platform                          () const;
  const std::map<di::hmd*                    , entity*>& hmd_entity_map                    () const;
  const std::map<di::vr_controller*          , entity*>& controller_entity_map             () const;
  const std::map<di::tracking_reference*     , entity*>& tracking_reference_entity_map     () const;
  const std::map<di::display_redirect*       , entity*>& display_redirect_entity_map       () const;
  const std::map<di::generic_tracking_device*, entity*>& generic_tracking_device_entity_map() const;

protected:
  void prepare(                             scene* scene) override;
  void update (frame_timer::duration delta, scene* scene) override;
  
  entity*                                         platform_                          = nullptr;                                      
  std::map<di::hmd*                    , entity*> hmd_entity_map_                    ;
  std::map<di::vr_controller*          , entity*> controller_entity_map_             ;
  std::map<di::tracking_reference*     , entity*> tracking_reference_entity_map_     ;
  std::map<di::display_redirect*       , entity*> display_redirect_entity_map_       ;
  std::map<di::generic_tracking_device*, entity*> generic_tracking_device_entity_map_;
};
}

#endif