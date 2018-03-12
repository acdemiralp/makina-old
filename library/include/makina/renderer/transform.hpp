#ifndef MAKINA_RENDERER_TRANSFORM_HPP_
#define MAKINA_RENDERER_TRANSFORM_HPP_

#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <makina/aspects/hierarchical.hpp>
#include <makina/renderer/coordinate_system.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT transform : public hierarchical<transform>
{
public:
  glm::vec3 translation       (bool global = false) const;
  glm::quat rotation          (bool global = false) const;
  glm::vec3 rotation_euler    (bool global = false) const;
  glm::vec3 scale             (bool global = false) const;
  glm::mat4 matrix            (bool global = false) const;
  
  glm::vec3 right             (bool global = false) const;
  glm::vec3 up                (bool global = false) const;
  glm::vec3 forward           (bool global = false) const;

  void      set_translation   (const glm::vec3& translation);
  void      set_rotation      (const glm::quat& rotation);
  void      set_rotation_euler(const glm::vec3& rotation);
  void      set_scale         (const glm::vec3& scale);
  void      set_matrix        (const glm::mat4& matrix);
                                                                           
  void      translate         (const glm::vec3& value);
  void      rotate            (const glm::quat& value);
  void      rotate_euler      (const glm::vec3& value);
  void      scale             (const glm::vec3& value);
  void      look_at           (const glm::vec3& target, const glm::vec3& up);
  void      reset             ();

protected:
  glm::vec3 translation_;
  glm::quat rotation_   ;
  glm::vec3 scale_      ;
  glm::mat4 matrix_     ;
};
}

#endif