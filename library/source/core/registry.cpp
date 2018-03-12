#include <makina/core/registry.hpp>

namespace mak
{
std::unique_ptr<settings::registry_type> registry = std::make_unique<settings::registry_type>();
}