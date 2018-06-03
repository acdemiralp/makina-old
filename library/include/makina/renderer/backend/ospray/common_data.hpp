#ifndef MAKINA_RENDERER_BACKEND_OSPRAY_COMMON_DATA_HPP_
#define MAKINA_RENDERER_BACKEND_OSPRAY_COMMON_DATA_HPP_

#include <memory>
#include <vector>

#include <fg/resource.hpp>
#include <ospray/ospray_cpp.h>

#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT common_data
{
  std::unique_ptr<ospray::cpp::Camera> camera;
  std::unique_ptr<ospray::cpp::Data>   lights;
};

struct MAKINA_EXPORT line_data
{
  struct MAKINA_EXPORT instance
  {
    std::unique_ptr<ospray::cpp::Geometry> geometry;
    std::unique_ptr<ospray::cpp::Data>     vertices;
    std::unique_ptr<ospray::cpp::Data>     colors  ;
    std::unique_ptr<ospray::cpp::Data>     indices ;
  };

  std::vector<instance> instances;
};

struct MAKINA_EXPORT mesh_data
{
  
};

struct MAKINA_EXPORT volume_data
{
  struct MAKINA_EXPORT instance
  {
    std::unique_ptr<ospray::cpp::Volume>           volume           ;
    std::unique_ptr<ospray::cpp::Data>             scalars          ;

    std::unique_ptr<ospray::cpp::TransferFunction> transfer_function;
    std::unique_ptr<ospray::cpp::Data>             colors           ;
    std::unique_ptr<ospray::cpp::Data>             opacities        ;
  };

  std::vector<instance> instances;
};
}

#endif