#ifndef MAKINA_RENDERER_BONE_HPP
#define MAKINA_RENDERER_BONE_HPP

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