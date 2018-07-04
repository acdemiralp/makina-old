#ifndef MAKINA_RESOURCES_DYNAMIC_MATERIAL_HPP
#define MAKINA_RESOURCES_DYNAMIC_MATERIAL_HPP

#include <any>
#include <map>

#include <makina/resources/material.hpp>
#include <makina/export.hpp>

namespace mak
{
struct MAKINA_EXPORT dynamic_material : material
{
  std::map<std::string, std::any> properties;
};
}

#endif