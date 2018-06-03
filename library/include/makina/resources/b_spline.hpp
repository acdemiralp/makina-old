#ifndef MAKINA_RESOURCES_B_SPLINE_HPP_
#define MAKINA_RESOURCES_B_SPLINE_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include <splinter/bspline.h>

#include <makina/resources/field.hpp>
#include <makina/resources/point_cloud.hpp>
#include <makina/resources/line_segments.hpp>
#include <makina/resources/mesh.hpp>
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

  template<typename type, std::size_t dimensions>
  std::unique_ptr<field<type, dimensions>> to_field                     (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const
  {
    auto   field = std::make_unique<mak::field<type, dimensions>>();

    return field;
  }
  std::unique_ptr<point_cloud>             to_point_cloud               (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const;
  std::unique_ptr<line_segments>           to_line_segments             (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const;
  std::unique_ptr<mesh>                    to_mesh                      (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const;
  std::unique_ptr<point_cloud>             control_points_to_point_cloud() const;

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