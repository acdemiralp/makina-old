#ifndef MAKINA_RESOURCES_LINE_SEGMENTS_HPP_
#define MAKINA_RESOURCES_LINE_SEGMENTS_HPP_

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <ra/resource.hpp>

#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT line_segments : named, ra::resource<line_segments>
{
  std::vector<glm::vec3>     vertices;
  std::vector<glm::u8vec4>   colors  ;
  std::vector<std::uint32_t> indices ;
};
}

#endif