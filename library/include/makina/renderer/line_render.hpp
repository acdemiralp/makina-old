#ifndef MAKINA_RENDERER_LINE_RENDER_HPP
#define MAKINA_RENDERER_LINE_RENDER_HPP

#include <makina/export.hpp>

namespace mak
{
struct line_segments;
struct material;

struct MAKINA_EXPORT line_render
{
  line_segments* line_segments;
  material*      material     ;
};
}

#endif