#ifndef MAKINA_PHYSICS_MESH_COLLIDER_HPP_
#define MAKINA_PHYSICS_MESH_COLLIDER_HPP_

#include <makina/export.hpp>

namespace mak
{
struct mesh;

struct MAKINA_EXPORT mesh_collider
{
  mesh* mesh;
};
}

#endif