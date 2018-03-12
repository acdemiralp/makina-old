#ifndef MAKINA_RESOURCES_IMAGE_HPP_
#define MAKINA_RESOURCES_IMAGE_HPP_

#include <cstddef>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT image
{
public:

protected:
  std::size_t index_;
  std::size_t value_;
};
}

#endif