#ifndef MAKINA_MAKINA_HPP_
#define MAKINA_MAKINA_HPP_

#include <memory>

#include <makina/core/engine.hpp>
#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT std::unique_ptr<engine> make_default_engine();
}

#endif
