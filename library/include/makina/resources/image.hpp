#ifndef MAKINA_RESOURCES_IMAGE_HPP_
#define MAKINA_RESOURCES_IMAGE_HPP_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <fi/image.hpp>

namespace mak
{
using image = fi::image;
}

#endif