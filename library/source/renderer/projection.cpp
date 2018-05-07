#include <makina/renderer/projection.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace mak
{
void             projection::set_perspective(const float fov_y, const float aspect_ratio, std::array<float, 2> z_range)
{
  matrix_ = glm::perspective(glm::radians(fov_y), aspect_ratio, z_range[0], z_range[1]);
  mode_   = projection_mode::perspective;
}
void             projection::set_orthogonal (const di::rectangle<float> rectangle       , std::array<float, 2> z_range)
{
  matrix_ = glm::ortho      (rectangle.left, rectangle.right, rectangle.bottom, rectangle.top, z_range[0], z_range[1]);
  mode_   = projection_mode::orthogonal;
}
void             projection::set_frustum    (const di::rectangle<float> rectangle       , std::array<float, 2> z_range)
{
  matrix_ = glm::frustum    (rectangle.left, rectangle.right, rectangle.bottom, rectangle.top, z_range[0], z_range[1]);
  mode_   = projection_mode::frustum;
}

const glm::mat4& projection::matrix         () const
{
  return matrix_;
}
projection::projection_mode projection::mode() const
{
  return mode_;
}
}
