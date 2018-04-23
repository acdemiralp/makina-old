#ifndef MAKINA_RENDERER_BONE_HPP_
#define MAKINA_RENDERER_BONE_HPP_

#include <cstdint>

#include <glm/mat4x4.hpp>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT bone
{
  std::uint32_t index;
  glm::mat4     offset_matrix;
};
}

#endif