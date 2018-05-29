#ifndef MAKINA_RENDERER_POINT_RENDER_HPP_
#define MAKINA_RENDERER_POINT_RENDER_HPP_

#include <makina/export.hpp>

namespace mak
{
struct point_cloud;

struct MAKINA_EXPORT point_render
{
  point_cloud* point_cloud;
};
}

#endif