#ifndef MAKINA_RESOURCES_FIELD_HPP_
#define MAKINA_RESOURCES_FIELD_HPP_

#include <array>
#include <cstddef>

#include <boost/multi_array.hpp>
#include <ra/resource.hpp>

#include <makina/aspects/named.hpp>
#include <makina/export.hpp>

namespace mak
{
template <typename type, std::size_t dimensions>
struct MAKINA_EXPORT field : named, ra::resource<field<type, dimensions>>
{
  boost::multi_array<type , dimensions> data   ;
  std::array        <float, dimensions> spacing;
};
}

#endif