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
  std::array<std::size_t, dimensions> locate(const position_type& position) const
  {
    std::array<std::size_t, dimensions> index;
    for (auto i = 0; i < dimensions; ++i)
      index[i] = position[i] / spacing[i];
    return index;
  }
  template<typename position_type>
  const type&                         get   (const position_type& position) const
  {
    return data(locate<position_type>(position));
  }

  boost::multi_array<type , dimensions> data   ;
  std::array        <float, dimensions> spacing;
};
}

#endif