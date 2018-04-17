#ifndef MAKINA_RESOURCES_ANIMATION_CURVE_HPP_
#define MAKINA_RESOURCES_ANIMATION_CURVE_HPP_

#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <makina/resources/keyframe.hpp>
#include <makina/export.hpp>

namespace mak
{
template<typename parameter_type = glm::vec3, typename temporal_type = float>
struct MAKINA_EXPORT animation_curve
{
  parameter_type interpolate(temporal_type time, const bool spherical = false)
  {
    for (auto i = 0; i < keyframes.size(); ++i)
    {
      if (keyframes[i].time < time && i < keyframes.size() - 1)
      {
        auto& start = keyframes[i];
        auto& end   = keyframes[i + 1];
        auto  t     = (time - start.time) / (end.time - start.time);
        return spherical ? glm::slerp(start.value, end.value, t) : glm::lerp(start.value, end.value, t);
      }
    }
    return parameter_type();
  }
  parameter_type mix        (temporal_type time)
  {
    for (auto i = 0; i < keyframes.size(); ++i)
    {
      if (keyframes[i].time < time && i < keyframes.size() - 1)
      {
        auto& start = keyframes[i];
        auto& end   = keyframes[i + 1];
        auto  t     = (time - start.time) / (end.time - start.time);
        return glm::mix(start.value, end.value, t);
      }
    }
    return parameter_type();
  }

  std::vector<keyframe<parameter_type, temporal_type>> keyframes;
};
}

#endif