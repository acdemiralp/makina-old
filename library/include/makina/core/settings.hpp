#ifndef MAKINA_CORE_SETTINGS_HPP_
#define MAKINA_CORE_SETTINGS_HPP_

#include <ratio>

#include <boost/mp11.hpp>

namespace mak
{
// Compile-time settings for the engine.
namespace settings
{
  using temporal_type   = float;
  using temporal_period = std::milli;
  using component_types = boost::mp11::mp_list<int, unsigned int, float, double>; // TODO: Change to actual components.
}
}

#endif