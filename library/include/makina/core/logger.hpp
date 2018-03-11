#ifndef MAKINA_CORE_LOGGER_HPP_
#define MAKINA_CORE_LOGGER_HPP_

#include <memory>

#include <spdlog/logger.h>

namespace mak
{
extern std::shared_ptr<spdlog::logger> logger;
}

#endif