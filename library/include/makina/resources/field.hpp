#ifndef MAKINA_RESOURCES_FIELD_HPP
#define MAKINA_RESOURCES_FIELD_HPP

#include <array>
#include <cstddef>

#include <boost/multi_array.hpp>
#include <ra/resource.hpp>

#include <makina/aspects/named.hpp>

namespace mak
{
template <typename type, std::size_t dimensions>
struct field : named, ra::resource<field<type, dimensions>>
{
  template<typename position_type>
  bool                                is_within  (const position_type& position) const
  {
    for (auto i = 0; i < dimensions; ++i)
    {
      const auto index = (position[i] + (cells ? 0 : spacing[i] / 2)) / spacing[i];
      if (0 > index || index >= data.shape()[i])
        return false;
    }
    return true;
  }
  template<typename position_type>
  std::array<std::size_t, dimensions> locate     (const position_type& position) const
  {
    std::array<std::size_t, dimensions> index;
    for (auto i = 0; i < dimensions; ++i)
      index[i] = std::floor((position[i] + (cells ? 0 : spacing[i] / 2)) / spacing[i]);
    return index;
  }

  template<typename position_type>             
        type&                         get        (const position_type& position)
  {
    return data(locate<position_type>(position));
  }
  template<typename position_type>             
  const type&                         get        (const position_type& position) const
  {
    return data(locate<position_type>(position));
  }

  template <typename position_type, typename weight_type = float>
  type                                interpolate(const position_type& position, const std::function<type(const type&, const type&, weight_type)>& function) const
  {
    type result;

    const auto index = locate<position_type>(position[i] + (cells_ ? -spacing[i] / 2 : 0));

    std::vector<type> intermediates; // Initially the intermediates are the 2^dimensions data points. Fill.
    for (auto i = 0; i < dimensions; ++i)
    {
      std::vector<type> next_intermediates(std::pow(2, dimensions - i - 1));

      // const auto a             = data(index);
      // const auto b             = data(index + 1);
      // const auto position_a    = 0.0f;
      // const auto position_b    = 0.0f;
      // const auto dist_to_a     = (position_a - position).norm();
      // const auto dist_to_b     = (position_b - position).norm();
      // const auto weight        = dist_to_a / (dist_to_a + dist_to_b);
      // result[i]                = (1 - weight) * a + weight * b;

      intermediates = next_intermediates;
    }

    return result;
  }

  boost::multi_array<type , dimensions> data    ;
  std::array        <float, dimensions> spacing ;
  bool                                  cells   = false; // Data on the cell or points?
};
}

#endif