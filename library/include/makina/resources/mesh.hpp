#ifndef MAKINA_RESOURCES_MESH_HPP_
#define MAKINA_RESOURCES_MESH_HPP_

#include <cstdint>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT mesh : public named
{
  std::vector<glm::vec3>     vertices           ;
  std::vector<glm::vec3>     normals            ;
  std::vector<glm::vec3>     texture_coordinates;
  std::vector<glm::u8vec4>   colors             ;
  std::vector<glm::vec3>     tangents           ;
  std::vector<glm::vec3>     bitangents         ;
  std::vector<std::uint32_t> indices            ;
  std::vector<glm::uvec4>    bone_ids           ;
  std::vector<glm::vec4>     bone_weights       ;
};
}

#endif