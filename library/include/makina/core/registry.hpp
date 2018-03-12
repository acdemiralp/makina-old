#ifndef MAKINA_CORE_REGISTRY_HPP_
#define MAKINA_CORE_REGISTRY_HPP_

#include <memory>

#include <ra/registry.hpp>

#include <makina/resources/image.hpp>
#include <makina/resources/model.hpp>
#include <makina/export.hpp>

namespace mak
{
using registry_type = ra::registry<image, model>;

MAKINA_EXPORT extern std::unique_ptr<registry_type> registry;
}

#endif