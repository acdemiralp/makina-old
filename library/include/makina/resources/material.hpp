#ifndef MAKINA_RESOURCES_MATERIAL_HPP_
#define MAKINA_RESOURCES_MATERIAL_HPP_

#include <any>
#include <map>
#include <memory>

#include <glm/vec3.hpp>

#include <makina/resources/image.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT material
{
    
};

struct MAKINA_EXPORT dynamic_material : material
{
  std::map<std::string, std::any> properties;
};

struct MAKINA_EXPORT physically_based_material : material
{
  physically_based_material           ()                                       = default;
  physically_based_material           (const physically_based_material&  that) = delete ;
  physically_based_material           (      physically_based_material&& temp) = default;
  virtual ~physically_based_material()                                         = default;
  physically_based_material& operator=(const physically_based_material&  that) = delete ;
  physically_based_material& operator=(      physically_based_material&& temp) = default;

  glm::vec3              albedo                 ;
  float                  metallicity            ;
  float                  roughness              ;
  float                  refractive_index       ;

  std::unique_ptr<image> albedo_image           ;
  std::unique_ptr<image> metallicity_image      ;
  std::unique_ptr<image> roughness_image        ;
  std::unique_ptr<image> normal_image           ;
  std::unique_ptr<image> ambient_occlusion_image;
};

struct MAKINA_EXPORT phong_material : material
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