#ifndef MAKINA_RESOURCES_REGISTRY_HPP_
#define MAKINA_RESOURCES_REGISTRY_HPP_

#include <ra/registry.hpp>

#include <makina/core/settings.hpp>
#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT extern ra::registry<settings::resource_types> registry;
}

#endif