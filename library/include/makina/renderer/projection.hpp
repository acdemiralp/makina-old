#ifndef MAKINA_RENDERER_PROJECTION_HPP_
#define MAKINA_RENDERER_PROJECTION_HPP_

#include <array>

#include <di/utility/rectangle.hpp>
#include <glm/mat4x4.hpp>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT projection
{
public:
  void             set_perspective(const float fov_y, const float aspect_ratio, std::array<float, 2> z_range);
  void             set_orthogonal (const di::rectangle<float> rectangle       , std::array<float, 2> z_range);
  const glm::mat4& matrix         () const;

protected:
  glm::mat4 matrix_;
};
}

#endif