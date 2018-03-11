#ifndef MAKINA_RESOURCES_MODEL_HPP_
#define MAKINA_RESOURCES_MODEL_HPP_

#include <memory>
#include <vector>

#include <makina/resources/material.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT model
{
public:
  model           ()                   = default;
  model           (const model&  that) = delete ;
  model           (      model&& temp) = default;
  virtual ~model  ()                   = default;
  model& operator=(const model&  that) = delete ;
  model& operator=(      model&& temp) = default;

  template<typename... argument_types>
  material*              add_material   (argument_types&&... arguments)
  {
    materials_.push_back(std::make_unique<material>(arguments...));
    return materials_.back().get();
  }
  template<typename... argument_types>
  mesh*                  add_mesh       (argument_types&&... arguments)
  {
    meshes_.push_back(std::make_unique<mesh>(arguments...));
    return meshes_.back().get();
  }

  void                   remove_material(material* material);
  void                   remove_mesh    (mesh*     mesh    );

  std::vector<material*> materials      () const;
  std::vector<mesh*>     meshes         () const;

protected:
  std::vector<std::unique_ptr<material>> materials_;
  std::vector<std::unique_ptr<mesh>>     meshes_   ;
};
}

#endif