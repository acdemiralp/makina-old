#ifndef MAKINA_RESOURCES_MODEL_HPP
#define MAKINA_RESOURCES_MODEL_HPP

#include <memory>
#include <string>
#include <vector>

#include <ra/resource.hpp>

#include <makina/aspects/named.hpp>
#include <makina/core/scene.hpp>
#include <makina/resources/animation_clip.hpp>
#include <makina/resources/material.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT model : named, ra::resource<model>
{
  struct description
  {
    std::string filepath      ;
    bool        pbr_materials = false;
  };

  model           ()                   = default;
  model           (const model&  that) = delete ;
  model           (      model&& temp) = default;
  virtual ~model  ()                   = default;
  model& operator=(const model&  that) = delete ;
  model& operator=(      model&& temp) = default;
  
  std::vector<std::unique_ptr<animation_clip>> animation_clips;
  std::vector<std::unique_ptr<material>>       materials      ;
  std::vector<std::unique_ptr<mesh>>           meshes         ;
  std::unique_ptr<scene>                       scene          ; // Models may contain partial scenes.
};
}

#endif