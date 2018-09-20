#ifndef MAKINA_RESOURCES_FIELD_HPP
#define MAKINA_RESOURCES_FIELD_HPP

#include <array>
#include <cstddef>

#include <boost/multi_array.hpp>
#include <ra/resource.hpp>

#include <makina/aspects/named.hpp>
#include <makina/utility/permute_for.hpp>

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
      const auto index = (position[i] + (cells ? 0 : (spacing[i] / 2))) / spacing[i];
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
      index[i] = std::floor((position[i] + (cells ? 0 : (spacing[i] / 2))) / spacing[i]);
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
  type                                interpolate(const position_type& position, const std::function<type(const type&, const type&, weight_type)>& function = [ ] (const type& a, const type& b, weight_type w) { return (1 - w) * a + w * b; }) const
  {
    std::array<std::size_t, dimensions> start_index, end_index, increment;
    for (auto i = 0; i < dimensions; ++i)
    {
      start_index[i] = std::floor((position[i] - (cells ? (spacing[i] / 2) : 0)) / spacing[i]);
      end_index  [i] = start_index[i] + 2;
      increment  [i] = 1;
    }

    std::vector<type> intermediates;
    intermediates.reserve(std::pow(2, dimensions));
    permute_for<dimensions>(
      [&] (const std::array<std::size_t, dimensions>& iteratee)
      {
        intermediates.push_back(data(iteratee));
      }, 
      start_index, 
      end_index  , 
      increment  );

    for (std::ptrdiff_t i = dimensions - 1; i >= 0; --i)
    {
      const auto weight = std::fmod(position[i], spacing[i]) / spacing[i];

      std::vector<type> next_intermediates;
      for (auto j = 0; j < std::pow(2, i); ++j)
        next_intermediates.push_back(function(intermediates[2 * j], intermediates[2 * j + 1], weight));
      intermediates = next_intermediates;
    }
    return intermediates[0];
  }

  boost::multi_array<type , dimensions> data    ;
  std::array        <float, dimensions> spacing ;
  bool                                  cells   = false; // Data on the cell or points?
};
}

#endif