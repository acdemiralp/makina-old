#ifndef MAKINA_RENDERER_MESH_RENDER_HPP_
#define MAKINA_RENDERER_MESH_RENDER_HPP_

#include <makina/export.hpp>

namespace mak
{
class material;
class mesh;

struct MAKINA_EXPORT mesh_render
{
  material* material;
  mesh*     mesh    ;
};
}

#endif