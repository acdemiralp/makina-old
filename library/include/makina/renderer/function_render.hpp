#ifndef MAKINA_RENDERER_FUNCTION_RENDER_HPP_
#define MAKINA_RENDERER_FUNCTION_RENDER_HPP_

#include <cstddef>
#include <functional>
#include <vector>

#include <splinter/datatable.h>
#include <splinter/bspline.h>
#include <splinter/bsplinebuilder.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT function_render
{
  void build () const
  {
    SPLINTER::DataTable samples;
    for (auto i = 0; i < resolution; ++i)
    {
      for (auto j = 0; j < resolution; ++j)
      {
        const std::vector<double> x {double(i), double(j)};
        samples.addSample(x, function(x));
      }
    }
    auto bspline = SPLINTER::BSpline::Builder(samples).degree(1).build();
    bspline.eval(std::vector<double>{0.0, 1.0});
  }

  std::function<double(std::vector<double>)> function  ;
  std::size_t                                resolution;
};
}

#endif