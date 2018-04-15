#ifndef MAKINA_RESOURCES_KEYFRAME_HPP_
#define MAKINA_RESOURCES_KEYFRAME_HPP_

#include <makina/export.hpp>

namespace mak
{
template<typename type = float>
struct MAKINA_EXPORT keyframe
{
  type time        = type(0);
  type value       = type(0);
  type in_tangent  = type(0);
  type out_tangent = type(0);
};
}

#endif