#include <makina/resources/b_spline.hpp>

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
