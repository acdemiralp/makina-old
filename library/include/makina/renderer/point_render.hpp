#ifndef MAKINA_RENDERER_POINT_RENDER_HPP
#define MAKINA_RENDERER_POINT_RENDER_HPP

#include <makina/export.hpp>

namespace mak
{
struct point_cloud;
struct material;

struct MAKINA_EXPORT point_render
{
  point_cloud* point_cloud;
  material*    material   ;
};
}

#endif