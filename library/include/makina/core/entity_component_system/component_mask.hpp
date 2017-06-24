#ifndef MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_COMPONENT_MASK_HPP
#define MAKINA_CORE_ENTITY_COMPONENT_SYSTEM_COMPONENT_MASK_HPP

#include <boost/dynamic_bitset.hpp>

namespace mak
{
template<typename precision>
using component_mask = boost::dynamic_bitset<precision>;
}

#endif