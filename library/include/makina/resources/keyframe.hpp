#ifndef MAKINA_RESOURCES_KEYFRAME_HPP_
#define MAKINA_RESOURCES_KEYFRAME_HPP_

#include <makina/export.hpp>

namespace mak
{
template<typename parameter_type, typename temporal_type>
struct MAKINA_EXPORT keyframe
{
  temporal_type  time ;
  parameter_type value;
};
}

#endif