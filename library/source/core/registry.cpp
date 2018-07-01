#include <makina/core/registry.hpp>

namespace mak
{
std::unique_ptr<registry_type> registry = std::make_unique<registry_type>();
}