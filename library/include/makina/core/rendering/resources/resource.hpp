#ifndef MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_HPP_
#define MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_HPP_

#include <cstddef>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT resource
{
public:
  class MAKINA_EXPORT description
  {

  };
};

struct MAKINA_EXPORT resource_handle
{
  std::size_t id;
};
}

#endif
