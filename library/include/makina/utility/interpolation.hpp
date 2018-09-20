#ifndef MAKINA_UTILITY_INTERPOLATION_HPP
#define MAKINA_UTILITY_INTERPOLATION_HPP

#include <cmath>
#include <numeric>

namespace mak
{
template<typename type, typename weight_type>
type lerp (const type& a, const type& b, weight_type w) 
{
  return (1 - w) * a + w * b;
}
template<typename type, typename weight_type>
type slerp(const type& a, const type& b, weight_type w)
{
  auto o = std::acos(std::inner_product(a.begin(), a.end(), b.begin(), 0));
  return (std::sin((1 - w) * o) / std::sin(o)) * a + (std::sin(w * o) / std::sin(o)) * b;
}
}

#endif