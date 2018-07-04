#ifndef MAKINA_RENDERER_MESH_RENDER_HPP
#define MAKINA_RENDERER_MESH_RENDER_HPP

#include <makina/export.hpp>

namespace mak
{
struct material;
struct mesh;

struct MAKINA_EXPORT mesh_render
{
  material* material;
  mesh*     mesh    ;
};
}

#endif