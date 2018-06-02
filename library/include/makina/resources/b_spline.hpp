#ifndef MAKINA_RESOURCES_B_SPLINE_HPP_
#define MAKINA_RESOURCES_B_SPLINE_HPP_

#include <cstddef>
#include <functional>
#include <tuple>
#include <vector>

#include <splinter/bspline.h>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT b_spline
{
public:
  explicit b_spline(
    const std::vector<std::tuple<std::vector<double>, double>>& table       ,
    const std::size_t                                           degree      = 1);
  explicit b_spline(
    const std::function<double(std::vector<double>)>&           function    ,
    const std::vector<double>&                                  lower_bounds,
    const std::vector<double>&                                  upper_bounds,
    const std::vector<std::size_t>&                             samples     ,
    const std::size_t                                           degree      = 1);
  b_spline           (const b_spline&  that) = default;
  b_spline           (      b_spline&& temp) = default;
  virtual ~b_spline  ()                      = default;
  b_spline& operator=(const b_spline&  that) = default;
  b_spline& operator=(      b_spline&& temp) = default;

  double evaluate(const std::vector<double>& parameters) const;

  // to point cloud                (2d-3d)
  // to line segment               (2d-3d)
  // to mesh                       (3d)
  // control points to point cloud (2d-3d)

protected:
  SPLINTER::BSpline from_table(
    const std::vector<std::tuple<std::vector<double>, double>>& table       ,
    const std::size_t                                           degree      ) const;
  SPLINTER::BSpline from_function(
    const std::function<double(std::vector<double>)>&           function    ,
    const std::vector<double>&                                  lower_bounds,
    const std::vector<double>&                                  upper_bounds,
    const std::vector<std::size_t>&                             samples     ,
    const std::size_t                                           degree      ) const;

  SPLINTER::BSpline native_;
};
}

#endif