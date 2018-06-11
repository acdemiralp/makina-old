#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_BUFFER_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_BUFFER_HPP_

#include <cstdint>

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/export.hpp>

namespace mak
{
namespace ospray
{
struct MAKINA_EXPORT buffer_description
{
  void*        data   ;
  std::size_t  size   ;
  OSPDataType  type   = OSP_FLOAT4;
  bool         shared = false     ;
};

using buffer_resource = fg::resource<buffer_description, ::ospray::cpp::Data>;
}
}

template<>
inline std::unique_ptr<ospray::cpp::Data> fg::realize(const mak::ospray::buffer_description& description)
{ 
  return std::make_unique<ospray::cpp::Data>(description.size, description.type, description.data, description.shared ? OSP_DATA_SHARED_BUFFER : 0);
}

#endif