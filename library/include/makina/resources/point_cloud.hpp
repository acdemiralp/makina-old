#ifndef MAKINA_RESOURCES_POINT_CLOUD_HPP_
#define MAKINA_RESOURCES_POINT_CLOUD_HPP_

#include <vector>

#include <glm/glm.hpp>

#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT point_cloud : named
{
  std::vector<glm::vec3>   vertices;
  std::vector<glm::u8vec4> colors  ;
};
}

#endif