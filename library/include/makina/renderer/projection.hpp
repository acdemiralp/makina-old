#ifndef MAKINA_RENDERER_PROJECTION_HPP_
#define MAKINA_RENDERER_PROJECTION_HPP_

#include <array>
#include <variant>

#include <di/utility/rectangle.hpp>
#include <glm/mat4x4.hpp>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT projection
{
public:
  enum class projection_mode
  {
    custom      ,
    perspective ,
    orthographic,
    frustum     // For HMDs.
  };

  struct perspective
  {
    float                fov_y       ;
    float                aspect_ratio;
    std::array<float, 2> z_range     ;
  };
  struct orthographic
  {
    di::rectangle<float> rectangle   ;
    std::array<float, 2> z_range     ;
  };
  struct frustum
  {
    di::rectangle<float> rectangle   ;
    std::array<float, 2> z_range     ;
  };
  using  parameter_type = std::variant<perspective, orthographic, frustum>;

  void             set_perspective (const float fov_y, const float aspect_ratio, std::array<float, 2> z_range);
  void             set_orthographic(const di::rectangle<float> rectangle       , std::array<float, 2> z_range);
  void             set_frustum     (const di::rectangle<float> rectangle       , std::array<float, 2> z_range);

  parameter_type   parameters      () const;
  projection_mode  mode            () const;
  const glm::mat4& matrix          () const;

protected:
  parameter_type parameters_;
  glm::mat4      matrix_    ;
};
}

#endif