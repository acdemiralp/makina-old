#ifndef MAKINA_CORE_OPERATING_SYSTEM_HPP_
#define MAKINA_CORE_OPERATING_SYSTEM_HPP_

namespace mak
{
enum class operating_system
{
  linux,
  macos,
  windows
};

operating_system current_operating_system()
{
#if defined __linux__
  return operating_system::linux;
#elif defined __APPLE__
  return operating_system::macos;
#elif defined _WIN32 || defined _WIN64
  return operating_system::windows;
#else
  #error "Unknown operating system."
#endif
}
}

#endif