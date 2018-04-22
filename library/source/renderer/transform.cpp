#include <makina/renderer/transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace mak
{
transform::transform(mak::metadata* metadata) : metadata_(metadata)
{
  update_matrix();
}

glm::vec3 transform::translation       (const bool absolute) const
{
  return absolute && parent_ ? parent_->translation(true) + translation_ : translation_;
}
glm::quat transform::rotation          (const bool absolute) const
{
  return absolute && parent_ ? parent_->rotation   (true) * rotation_    : rotation_;
}
glm::vec3 transform::rotation_euler    (const bool absolute) const
{
  return glm::degrees(glm::eulerAngles(rotation(absolute)));
}
glm::vec3 transform::scale             (const bool absolute) const
{
  return absolute && parent_ ? parent_->scale      (true) * scale_       : scale_;
}
glm::mat4 transform::matrix            (const bool absolute) const
{
  return absolute ? absolute_matrix_ : matrix_;
}
          
glm::vec3 transform::right             (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(1.0f, 0.0f, 0.0f);
}
glm::vec3 transform::up                (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::vec3 transform::forward           (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(0.0f, 0.0f, 1.0f);
}
          
void      transform::set_translation   (const glm::vec3& translation                , const bool absolute)
{
  translation_ = absolute && parent_ ? translation - parent_->translation(true) : translation;
  update_matrix();
}
void      transform::set_rotation      (const glm::quat& rotation                   , const bool absolute)
{
  rotation_ = absolute && parent_ ? glm::inverse(parent_->rotation(true)) * rotation : rotation;
  update_matrix();
}
void      transform::set_rotation_euler(const glm::vec3& rotation                   , const bool absolute)
{
  set_rotation(glm::quat(glm::radians(rotation)), absolute);
}
void      transform::set_scale         (const glm::vec3& scale                      , const bool absolute)
{
  scale_ = absolute && parent_ ? scale / parent_->scale(true) : scale;
  update_matrix();
}
void      transform::set_matrix        (const glm::mat4& matrix                     , const bool absolute)
{
  glm::vec3 skew       ;
  glm::vec4 perspective;
  matrix_ = absolute && parent_ ? glm::inverse(parent_->absolute_matrix_) * matrix : matrix;
  glm::decompose(matrix_, scale_, rotation_, translation_, skew, perspective);
  update_hierarchy();
}
          
void      transform::translate         (const glm::vec3& value                      , const bool absolute)
{
  set_translation(value + translation(absolute), absolute);
}
void      transform::rotate            (const glm::quat& value                      , const bool absolute)
{
  set_rotation   (value * rotation   (absolute), absolute);
}
void      transform::rotate_euler      (const glm::vec3& value                      , const bool absolute)
{
  rotate         (glm::quat(glm::radians(value)), absolute);
}
void      transform::scale             (const glm::vec3& value                      , const bool absolute)
{
  set_scale      (value * scale      (absolute), absolute);
}
void      transform::look_at           (const glm::vec3& target, const glm::vec3& up, const bool absolute)
{
  set_rotation(glm::conjugate(glm::toQuat(glm::lookAt(translation(absolute), target, up))), absolute);
}
void      transform::reset             ()
{
  translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation_    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  scale_       = glm::vec3(1.0f, 1.0f, 1.0f);
  update_matrix();
}
          
void      transform::set_parent        (transform* parent)
{
  set_parent(parent, true);
}
void      transform::set_parent        (transform* parent, const bool maintain_absolute)
{
  if (parent_)
    parent_->children_.erase(std::remove_if(
      parent_->children_.begin(), 
      parent_->children_.end  (), 
      [&] (transform* child) {return child->metadata_->name == metadata_->name; }), 
      parent_->children_.end  ());   
  parent_ = parent;
  if (parent_)
    parent_->children_.push_back(this);

  if (maintain_absolute) set_matrix(absolute_matrix_, true);
  update_matrix();
}
          
metadata* transform::metadata          () const
{
  return metadata_;
}
void      transform::set_metadata      (mak::metadata* metadata)
{
  metadata_ = metadata;
}

void      transform::update_matrix     ()
{
  matrix_ = glm::translate(translation_) * glm::mat4_cast(rotation_) * glm::scale(scale_);
  update_hierarchy();
}
void      transform::update_hierarchy  ()
{
  absolute_matrix_ = parent_ ? parent_->absolute_matrix_ * matrix_ : matrix_;
  for (auto& child : children_) 
    child->update_hierarchy();
}
}
