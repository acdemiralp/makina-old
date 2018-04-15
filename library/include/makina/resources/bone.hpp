#ifndef MAKINA_RESOURCES_BONE_HPP_
#define MAKINA_RESOURCES_BONE_HPP_

#include <glm/mat4x4.hpp>

#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT bone : public named
{
  glm::mat4 offset_matrix;
};
}

#endif