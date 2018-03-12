#ifndef MAKINA_CORE_REGISTRY_HPP_
#define MAKINA_CORE_REGISTRY_HPP_

#include <memory>

#include <ra/registry.hpp>

#include <makina/core/settings.hpp>
#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT extern std::unique_ptr<ra::registry<settings::resource_types>> registry;
}

#endif