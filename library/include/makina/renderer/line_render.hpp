#ifndef MAKINA_RENDERER_LINE_RENDER_HPP_
#define MAKINA_RENDERER_LINE_RENDER_HPP_

#include <makina/export.hpp>

namespace mak
{
struct line_segments;

struct MAKINA_EXPORT line_render
{
  line_segments* line_segments;
};
}

#endif