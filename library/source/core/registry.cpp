#include <makina/core/registry.hpp>

namespace mak
{
std::unique_ptr<ra::registry<settings::resource_types>> registry = std::make_unique<ra::registry<settings::resource_types>>();
}