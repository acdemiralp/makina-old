#ifndef MAKINA_RESOURCES_TRANSFER_FUNCTION_HPP_
#define MAKINA_RESOURCES_TRANSFER_FUNCTION_HPP_

#include <vector>

#include <glm/glm.hpp>
#include <ra/resource.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT transfer_function : ra::resource<transfer_function>
{
  std::vector<glm::vec3> colors     ;
  std::vector<float>     opacities  ;
  glm::vec2              value_range;
};
}

#endif