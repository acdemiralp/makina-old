#include <makina/resources/b_spline.hpp>

#include <cstddef>
#include <cstdint>

#include <splinter/bsplinebuilder.h>
#include <splinter/datatable.h>

#include <makina/utility/indexing.hpp>

namespace mak
{
b_spline::b_spline(
  const std::vector<std::tuple<std::vector<double>, double>>& table       , 
  const std::uint32_t                                         degree      )
: native_(from_table(table, degree))
{

}
b_spline::b_spline(
  const std::function<double(std::vector<double>)>&           function    , 
  const std::vector<double>&                                  lower_bounds, 
  const std::vector<double>&                                  upper_bounds, 
  const std::vector<std::size_t>&                             samples     , 
  const std::uint32_t                                         degree      )
: native_(from_function(function, lower_bounds, upper_bounds, samples, degree))
{

}

double                         b_spline::evaluate                     (const std::vector<double>& parameters) const
{
  return native_.eval(parameters);
}

std::unique_ptr<point_cloud>   b_spline::to_point_cloud               (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const
{
  auto point_cloud = std::make_unique<mak::point_cloud>();

  std::vector<double> step_sizes(lower_bounds.size(), 0.0);
  for (std::size_t i = 0; i < step_sizes.size(); ++i)
    step_sizes[i] = (upper_bounds[i] - lower_bounds[i]) / samples[i];

  const auto sample_count = std::accumulate(samples.begin(), samples.end(), 1, std::multiplies<std::size_t>());
  point_cloud->vertices.resize(sample_count);
  point_cloud->colors  .resize(sample_count, {255, 255, 255, 255});
  mak::permute_for(
    [&] (const std::vector<std::size_t>& indices)
    { 
      std::vector<double> parameters(indices.size(), 0.0);
      for (std::size_t i = 0; i < parameters.size(); ++i)
        parameters[i] = lower_bounds[i] + step_sizes[i] * indices[i];
      parameters.push_back(evaluate(parameters));

      std::copy_n(parameters.data(), std::min(std::int32_t(parameters.size()), 3), &point_cloud->vertices[ravel_multi_index(indices, samples)][0]);
    },
    std::vector<std::size_t>(samples.size(), 0),
    samples,
    std::vector<std::size_t>(samples.size(), 1));

  return point_cloud;
}
std::unique_ptr<line_segments> b_spline::to_line_segments             (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const
{
  auto line_segments = std::make_unique<mak::line_segments>();

  std::vector<double> step_sizes(lower_bounds.size(), 0.0);
  for (std::size_t i = 0; i < step_sizes.size(); ++i)
    step_sizes[i] = (upper_bounds[i] - lower_bounds[i]) / samples[i];
  
  const auto sample_count = std::accumulate(samples.begin(), samples.end(), 1, std::multiplies<std::size_t>());
  line_segments->vertices.resize(     sample_count);
  line_segments->colors  .resize(     sample_count, {255, 255, 255, 255});
  line_segments->indices .resize(2 * (sample_count - 1));
  mak::permute_for(
    [&] (const std::vector<std::size_t>& indices)
    { 
      std::vector<double> parameters(indices.size(), 0.0);
      for (std::size_t i = 0; i < parameters.size(); ++i)
        parameters[i] = lower_bounds[i] + step_sizes[i] * indices[i];
      parameters.push_back(evaluate(parameters));

      auto index = ravel_multi_index(indices, samples);
      std::copy_n(parameters.data(), std::min(std::int32_t(parameters.size()), 3), &line_segments->vertices[index][0]);
      if (index != sample_count - 1)
      {
        line_segments->indices[2 * index]     = static_cast<std::uint32_t>(index);
        line_segments->indices[2 * index + 1] = static_cast<std::uint32_t>(index + 1);
      }
    },
    std::vector<std::size_t>(samples.size(), 0),
    samples,
    std::vector<std::size_t>(samples.size(), 1));

  return line_segments;
}
std::unique_ptr<mesh>          b_spline::to_mesh                      (const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds, const std::vector<std::size_t>& samples) const
{
  auto mesh = std::make_unique<mak::mesh>();
  
  std::vector<double> step_sizes(lower_bounds.size(), 0.0);
  for (std::size_t i = 0; i < step_sizes.size(); ++i)
    step_sizes[i] = (upper_bounds[i] - lower_bounds[i]) / samples[i];
  
  const auto sample_count = std::accumulate(samples.begin(), samples.end(), 1, std::multiplies<std::size_t>());
  mesh->vertices.resize(    sample_count);
  mesh->normals .resize(    sample_count, {0, 0, 0});
  mesh->colors  .resize(    sample_count, {255, 255, 255, 255});
  mesh->indices .resize(6 * sample_count);
  mak::permute_for(
    [&] (const std::vector<std::size_t>& indices)
    { 
      std::vector<double> parameters(indices.size(), 0.0);
      for (std::size_t i = 0; i < parameters.size(); ++i)
        parameters[i] = lower_bounds[i] + step_sizes[i] * indices[i];
      parameters.push_back(evaluate(parameters));
  
      auto index = static_cast<std::uint32_t>(ravel_multi_index(indices, samples));    
      std::copy_n(parameters.data(), std::min(std::int32_t(parameters.size()), 3), &mesh->vertices[index][0]);
      if (indices[0] > 0 && indices[1] > 0)
      {
        auto indices_e  = indices; indices_e [0]--;
        auto indices_se = indices; indices_se[0]--; indices_se[1]--;
        auto indices_s  = indices; indices_s [1]--;
        auto index_e    = static_cast<std::uint32_t>(ravel_multi_index(indices_e , samples));
        auto index_se   = static_cast<std::uint32_t>(ravel_multi_index(indices_se, samples));
        auto index_s    = static_cast<std::uint32_t>(ravel_multi_index(indices_s , samples));
        mesh->indices[6 * index + 0] = index_se;
        mesh->indices[6 * index + 1] = index_s ;
        mesh->indices[6 * index + 2] = index   ;
        mesh->indices[6 * index + 3] = index_se;
        mesh->indices[6 * index + 4] = index   ;
        mesh->indices[6 * index + 5] = index_e ;

        auto x = normalize(mesh->vertices[index_s] - mesh->vertices[index_se]);
        auto y = normalize(mesh->vertices[index_e] - mesh->vertices[index_se]);
        mesh->normals[index] = cross(x, y); // TODO: Mix the face normals into smooth vertex normals.
      }
    },
    std::vector<std::size_t>(samples.size(), 0),
    samples,
    std::vector<std::size_t>(samples.size(), 1));

  return mesh;
}
std::unique_ptr<point_cloud>   b_spline::control_points_to_point_cloud() const
{
  auto point_cloud    = std::make_unique<mak::point_cloud>();
  auto control_points = native_.getControlPoints();
  
  point_cloud->vertices.resize(control_points.rows());
  point_cloud->colors  .resize(control_points.rows(), {255, 255, 255, 255});
  for (auto i = 0; i < control_points.rows(); ++i)
  {
    SPLINTER::DenseVector row = control_points.row(i);
    std::copy_n(row.data(), std::min(std::int32_t(control_points.cols()), 3), &point_cloud->vertices[i][0]);
  }

  return point_cloud;
}

SPLINTER::BSpline b_spline::from_table(
  const std::vector<std::tuple<std::vector<double>, double>>& table       ,
  const std::uint32_t                                         degree      ) const
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
  const std::uint32_t                                         degree      ) const
{
  std::vector<double> step_sizes(lower_bounds.size(), 0.0);
  for (std::size_t i = 0; i < step_sizes.size(); ++i)
    step_sizes[i] = (upper_bounds[i] - lower_bounds[i]) / samples[i];

  SPLINTER::DataTable data_table;
  mak::permute_for(
    [&] (const std::vector<std::size_t>& indices)
    {
      std::vector<double> parameters(indices.size(), 0.0);
      for (std::size_t i = 0; i < parameters.size(); ++i)
        parameters[i] = lower_bounds[i] + step_sizes[i] * indices[i];
      data_table.addSample(parameters, function(parameters));
    }, 
    std::vector<std::size_t>(samples.size(), 0), 
    samples                                    ,
    std::vector<std::size_t>(samples.size(), 1));
  return SPLINTER::BSpline::Builder(data_table).degree(degree).build();
}
}
