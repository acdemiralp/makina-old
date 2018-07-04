#ifndef MAKINA_DEFAULT_ENGINE_HPP
#define MAKINA_DEFAULT_ENGINE_HPP

#include <memory>

#include <makina/core/engine.hpp>
#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT std::unique_ptr<engine> make_default_engine();
}

#endif
