#ifndef MAKINA_CORE_UTILITY_GPU_EXPORTS_HPP_
#define MAKINA_CORE_UTILITY_GPU_EXPORTS_HPP_

extern "C" 
{
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;

  _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#endif
