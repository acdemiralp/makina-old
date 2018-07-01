#ifndef MAKINA_RESOURCES_ANIMATION_CURVE_HPP_
#define MAKINA_RESOURCES_ANIMATION_CURVE_HPP_

#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/vec3.hpp>

#include <makina/resources/keyframe.hpp>
#include <makina/export.hpp>

namespace mak
{
template<typename parameter_type = glm::vec3, typename temporal_type = frame_timer::duration>
struct MAKINA_EXPORT animation_curve
{
  parameter_type lerp (temporal_type time)
  {
    for (auto i = 1; i < keyframes.size(); ++i)
    {
      if (time < keyframes[i].time)
      {
        auto& start = keyframes[i - 1];
        auto& end   = keyframes[i];
        auto  t     = (time - start.time) / (end.time - start.time);
        return glm::lerp(start.value, end.value, t);
      }
    }
    return parameter_type();
  }
  parameter_type slerp(temporal_type time)
  {
    for (auto i = 1; i < keyframes.size(); ++i)
    {
      if (time < keyframes[i].time)
      {
        auto& start = keyframes[i - 1];
        auto& end   = keyframes[i];
        auto  t     = (time - start.time) / (end.time - start.time);
        return glm::slerp(start.value, end.value, t);
      }
    }
    return parameter_type();
  }

  std::vector<keyframe<parameter_type, temporal_type>> keyframes;
};
}

#endif