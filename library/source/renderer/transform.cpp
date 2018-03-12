#include <makina/renderer/transform.hpp>

namespace mak
{
glm::vec3 transform::translation       (bool global) const
{
  
}
glm::quat transform::rotation          (bool global) const
{
  
}
glm::vec3 transform::rotation_euler    (bool global) const
{
  
}
glm::vec3 transform::scale             (bool global) const
{
  
}
glm::mat4 transform::matrix            (bool global) const
{
  
}

glm::vec3 transform::right             (bool global) const
{
  
}
glm::vec3 transform::up                (bool global) const
{
  
}
glm::vec3 transform::forward           (bool global) const
{
  
}

void      transform::set_translation   (const glm::vec3& translation)
{
  
}
void      transform::set_rotation      (const glm::quat& rotation)
{
  
}
void      transform::set_rotation_euler(const glm::vec3& rotation)
{
  
}
void      transform::set_scale         (const glm::vec3& scale)
{
  
}
void      transform::set_matrix        (const glm::mat4& matrix)
{
  
}
                                                                         
void      transform::translate         (const glm::vec3& value)
{
  
}
void      transform::rotate            (const glm::quat& value)
{
  
}
void      transform::rotate_euler      (const glm::vec3& value)
{
  
}
void      transform::scale             (const glm::vec3& value)
{
  
}
void      transform::look_at           (const glm::vec3& target, const glm::vec3& up)
{
  
}
void      transform::reset             ()
{
  
}
}