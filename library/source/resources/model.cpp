#include <makina/resources/model.hpp>

#include <algorithm>

namespace mak
{
void                   model::remove_material(material* material)
{
  auto iterator = std::remove_if(
    materials_.begin(), 
    materials_.end  (), 
    [material] (const std::unique_ptr<mak::material>& iteratee)
    {
      return iteratee.get() == material;
    });
}
void                   model::remove_mesh    (mesh*     mesh    )
{
  auto iterator = std::remove_if(
    meshes_.begin(), 
    meshes_.end  (), 
    [mesh] (const std::unique_ptr<mak::mesh>& iteratee)
    {
      return iteratee.get() == mesh;
    });
}

std::vector<material*> model::materials      () const
{
  std::vector<material*> materials(materials_.size());
  std::transform(
    materials_.begin(), 
    materials_.end  (), 
    materials .begin(), 
    [ ] (const std::unique_ptr<material>& iteratee)
    {
      return iteratee.get();
    });
  return materials;
}
std::vector<mesh*>     model::meshes         () const
{
  std::vector<mesh*> meshes(meshes_.size());
  std::transform(
    meshes_.begin(), 
    meshes_.end  (), 
    meshes .begin(), 
    [ ] (const std::unique_ptr<mesh>& iteratee)
    {
      return iteratee.get();
    });
  return meshes;
}
}
