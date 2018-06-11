#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_CAMERA_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_CAMERA_HPP_

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

namespace mak
{
namespace ospray
{
struct camera_description
{
  enum class type
  {
    perspective ,
    orthographic,
    panoramic   
  };

  type type;
};

using  camera_resource = fg::resource<camera_description, ::ospray::cpp::Camera>;
}
}

template<>
inline std::unique_ptr<ospray::cpp::Camera> fg::realize(const mak::ospray::camera_description& description)
{
  std::string type;
  if      (description.type == mak::ospray::camera_description::type::perspective ) type = "perspective" ;
  else if (description.type == mak::ospray::camera_description::type::orthographic) type = "orthographic";
  else if (description.type == mak::ospray::camera_description::type::panoramic   ) type = "panoramic"   ;
  return std::make_unique<ospray::cpp::Camera>(type);
}

#endif