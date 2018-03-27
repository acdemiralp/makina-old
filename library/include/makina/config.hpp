#ifndef MAKINA_CONFIG_HPP_
#define MAKINA_CONFIG_HPP_

#if defined __linux__
  #define VK_USE_PLATFORM_XLIB_KHR
#elif defined __APPLE__
  #define VK_USE_PLATFORM_XLIB_KHR
#elif defined _WIN32 || defined _WIN64
  #define VK_USE_PLATFORM_WIN32_KHR
#endif

#endif