#ifndef MAKINA_RESOURCES_PHYSICALLY_BASED_MATERIAL_HPP_
#define MAKINA_RESOURCES_PHYSICALLY_BASED_MATERIAL_HPP_

#include <memory>

#include <glm/vec3.hpp>

#include <makina/resources/image.hpp>
#include <makina/resources/material.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT physically_based_material : public material
{
  physically_based_material           ()                                       = default;
  physically_based_material           (const physically_based_material&  that) = delete ;
  physically_based_material           (      physically_based_material&& temp) = default;
  virtual ~physically_based_material  ()                                       = default;
  physically_based_material& operator=(const physically_based_material&  that) = delete ;
  physically_based_material& operator=(      physically_based_material&& temp) = default;

  glm::vec3              albedo                  = glm::vec3(1.0f);
  float                  metallicity             = 0.0f;
  float                  roughness               = 0.5f;

  std::unique_ptr<image> albedo_image            = nullptr;
  std::unique_ptr<image> metallicity_image       = nullptr;
  std::unique_ptr<image> roughness_image         = nullptr;
  std::unique_ptr<image> normal_image            = nullptr;
  std::unique_ptr<image> ambient_occlusion_image = nullptr;
};
}

#endif