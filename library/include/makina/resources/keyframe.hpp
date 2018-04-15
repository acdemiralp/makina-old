#ifndef MAKINA_RESOURCES_KEYFRAME_HPP_
#define MAKINA_RESOURCES_KEYFRAME_HPP_

#include <makina/export.hpp>

namespace mak
{
template<typename parameter_type = float, typename temporal_type = float>
struct MAKINA_EXPORT keyframe
{
  temporal_type  time        = temporal_type (0);
  parameter_type value       = parameter_type(0);
  parameter_type in_tangent  = parameter_type(0);
  parameter_type out_tangent = parameter_type(0);
};
}

#endif