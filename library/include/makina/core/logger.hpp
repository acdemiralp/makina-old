#ifndef MAKINA_CORE_LOGGER_HPP_
#define MAKINA_CORE_LOGGER_HPP_

#include <memory>

#include <spdlog/logger.h>

#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT extern std::shared_ptr<spdlog::logger> logger;
}

#endif