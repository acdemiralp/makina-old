#ifndef MAKINA_RESOURCES_PHONG_MATERIAL_HPP_
#define MAKINA_RESOURCES_PHONG_MATERIAL_HPP_

#include <memory>

#include <glm/vec3.hpp>

#include <makina/resources/image.hpp>
#include <makina/resources/material.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT phong_material : public material
{
  phong_material           ()                            = default;
  phong_material           (const phong_material&  that) = delete ;
  phong_material           (      phong_material&& temp) = default;
  virtual ~phong_material  ()                            = default;
  phong_material& operator=(const phong_material&  that) = delete ;
  phong_material& operator=(      phong_material&& temp) = default;

  glm::vec3              ambient       ;
  glm::vec3              diffuse       ;
  glm::vec3              specular      ;
  float                  shininess     ;

  std::unique_ptr<image> ambient_image ;
  std::unique_ptr<image> diffuse_image ;
  std::unique_ptr<image> specular_image;
};
}

#endif