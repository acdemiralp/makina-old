#ifndef MAKINA_RESOURCES_MODEL_HPP_
#define MAKINA_RESOURCES_MODEL_HPP_

#include <memory>
#include <vector>

#include <makina/aspects/named.hpp>
#include <makina/resources/material.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT model : public named
{
  model           ()                   = default;
  model           (const model&  that) = delete ;
  model           (      model&& temp) = default;
  virtual ~model  ()                   = default;
  model& operator=(const model&  that) = delete ;
  model& operator=(      model&& temp) = default;
  
  std::vector<std::unique_ptr<material>> materials;
  std::vector<std::unique_ptr<mesh>>     meshes   ;
};
}

#endif