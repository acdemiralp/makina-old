#ifndef MAKINA_UTILITY_ENABLE_DEDICATED_GPU_HPP
#define MAKINA_UTILITY_ENABLE_DEDICATED_GPU_HPP

#if _WIN32 || _WIN64
extern "C"
{
  __declspec(dllexport) int          AmdPowerXpressRequestHighPerformance = 1;
  __declspec(dllexport) unsigned int NvOptimusEnablement                  = 0x00000001;
}
#elif
{
                        int          AmdPowerXpressRequestHighPerformance = 1;
                        unsigned int NvOptimusEnablement                  = 0x00000001;
}
#endif

#endif