#ifndef MAKINA_RESOURCES_ANIMATION_CLIP_HPP_
#define MAKINA_RESOURCES_ANIMATION_CLIP_HPP_

#include <map>
#include <string>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <makina/aspects/named.hpp>
#include <makina/resources/animation_curve.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT animation_clip : public named
{
  enum class wrap_mode
  {
    once     ,
    loop     ,
    ping_pong,
    clamp  
  };

  float                                             framerate         ;
  float                                             length            ;
  wrap_mode                                         wrap_mode         ;
  std::map<std::string, animation_curve<glm::vec3>> translation_curves;
  std::map<std::string, animation_curve<glm::quat>> rotation_curves   ;
  std::map<std::string, animation_curve<glm::vec3>> scaling_curves    ;
};
}

#endif