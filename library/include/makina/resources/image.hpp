#ifndef MAKINA_RESOURCES_IMAGE_HPP
#define MAKINA_RESOURCES_IMAGE_HPP

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