#ifndef MAKINA_RENDERER_LIGHT_HPP_
#define MAKINA_RENDERER_LIGHT_HPP_

#include <glm/vec3.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT light
{
  enum class type
  {
    ambient    ,
    directional,
    point      ,
    spot
  };

  type      type       = type::directional ;
  glm::vec3 color      = {1.0f, 1.0f, 1.0f};
  float     intensity  = 1.0f ;
  float     range      = 10.0f; // Used only when type is point or spot.
  float     spot_angle = 45.0f; // Used only when type is spot.
};
}

#endif