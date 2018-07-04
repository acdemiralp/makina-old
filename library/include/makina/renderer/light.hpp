#ifndef MAKINA_RENDERER_LIGHT_HPP
#define MAKINA_RENDERER_LIGHT_HPP

#include <glm/vec2.hpp>
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

  type      type        = type::directional ;
  glm::vec3 color       = {1.0f, 1.0f, 1.0f};
  float     intensity   = 1.0f;
  float     range       = 100.0f;             // Used only when type is point or spot.
  glm::vec2 spot_angles = {45.0f, 55.0f};     // Used only when type is spot.
};
}

#endif