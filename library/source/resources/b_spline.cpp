#include <makina/resources/b_spline.hpp>

#include <cstdint>

#include <splinter/bsplinebuilder.h>
#include <splinter/datatable.h>

#include <makina/utility/permute_for.hpp>

namespace mak
{
b_spline::b_spline(
  const std::vector<std::tuple<std::vector<double>, double>>& table       , 
  const std::size_t                                           degree      )
: native_(from_table(table, degree))
{

}
b_spline::b_spline(
  const std::function<double(std::vector<double>)>&           function    , 
  const std::vector<double>&                                  lower_bounds, 
  const std::vector<double>&                                  upper_bounds, 
  const std::vector<std::size_t>&                             samples     , 
  const std::size_t                                           degree      )
: native_(from_function(function, lower_bounds, upper_bounds, samples, degree))
{

}

double b_spline::evaluate(const std::vector<double>& parameters) const
{
  return native_.eval(parameters);
}

std::unique_ptr<point_cloud>   b_spline::to_point_cloud               (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples)
{
  auto   point_cloud = std::make_unique<mak::point_cloud>();

  return point_cloud;
}
std::unique_ptr<line_segments> b_spline::to_line_segments             (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples)
{
  auto   line_segments = std::make_unique<mak::line_segments>();

  return line_segments;
}
std::unique_ptr<mesh>          b_spline::to_mesh                      (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples)
{
  auto   mesh = std::make_unique<mak::mesh>();

  return mesh;
}
std::unique_ptr<point_cloud>   b_spline::control_points_to_point_cloud()
{
  auto point_cloud    = std::make_unique<mak::point_cloud>();
  auto control_points = native_.getControlPoints();
  
  point_cloud->vertices.resize(control_points.cols());
  point_cloud->colors  .resize(control_points.cols(), {255, 255, 255, 255});
  for (auto i = 0; i < control_points.cols(); ++i)
    std::copy_n(control_points.col(i).data(), std::min(std::int32_t(control_points.rows()), 3), &point_cloud->vertices[i][0]);

  return point_cloud;
}

SPLINTER::BSpline b_spline::from_table(
  const std::vector<std::tuple<std::vector<double>, double>>& table       ,
  const std::size_t                                           degree      ) const
{
  SPLINTER::DataTable data_table;
  for (auto entry : table)
    data_table.addSample(std::get<0>(entry), std::get<1>(entry));
  return SPLINTER::BSpline::Builder(data_table).degree(degree).build();
}
SPLINTER::BSpline b_spline::from_function(
  const std::function<double(std::vector<double>)>&           function    ,
  const std::vector<double>&                                  lower_bounds,
  const std::vector<double>&                                  upper_bounds,
  const std::vector<std::size_t>&                             samples     ,
  const std::size_t                                           degree      ) const
{
  std::vector<double> step_sizes(lower_bounds.size(), 0.0);
  for (auto i = 0; i < step_sizes.size(); ++i)
    step_sizes[i] = (upper_bounds[i] - lower_bounds[i]) / samples[i];

  SPLINTER::DataTable data_table;
  mak::permute_for(
    [&] (const std::vector<std::size_t>& indices)
    {
      std::vector<double> parameters(indices.size(), 0.0);
      for (auto i = 0; i < parameters.size(); ++i)
        parameters[i] = lower_bounds[i] + step_sizes[i] * indices[i];
      data_table.addSample(parameters, function(parameters));
    }, 
    std::vector<std::size_t>(samples.size(), 0), 
    samples                                    ,
    std::vector<std::size_t>(samples.size(), 1));
  return SPLINTER::BSpline::Builder(data_table).degree(degree).build();
}
}
