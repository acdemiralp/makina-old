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
  enum class mode
  {
    custom      ,
    perspective ,
    orthographic,
    frustum     // For HMDs.
  };
  struct MAKINA_EXPORT perspective
  {
    float                fov_y       ;
    float                aspect_ratio;
    std::array<float, 2> z_range     ;
  };
  struct MAKINA_EXPORT orthographic
  {
    di::rectangle<float> rectangle   ;
    std::array<float, 2> z_range     ;
  };
  struct MAKINA_EXPORT frustum
  {
    di::rectangle<float> rectangle   ;
    std::array<float, 2> z_range     ;
  };

  void                                             set_perspective (const float fov_y, const float aspect_ratio, std::array<float, 2> z_range);
  void                                             set_orthographic(const di::rectangle<float> rectangle       , std::array<float, 2> z_range);
  void                                             set_frustum     (const di::rectangle<float> rectangle       , std::array<float, 2> z_range);
  
  mode                                             mode            () const;
  std::variant<perspective, orthographic, frustum> parameters      () const;
  const glm::mat4&                                 matrix          () const;

protected:
  std::variant<perspective, orthographic, frustum> parameters_;
  glm::mat4                                        matrix_    ;
};
}

#endif