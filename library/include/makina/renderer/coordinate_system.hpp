#ifndef MAKINA_RENDERER_COORDINATE_SYSTEM_HPP_
#define MAKINA_RENDERER_COORDINATE_SYSTEM_HPP_

#include <glm/vec3.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT coordinate_system
{
  glm::vec3 right  ;
  glm::vec3 up     ;
  glm::vec3 forward;
};

extern coordinate_system lh_coordinate_system;
extern coordinate_system rh_coordinate_system;
}

#endif