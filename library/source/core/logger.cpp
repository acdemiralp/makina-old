#include <makina/core/logger.hpp>

#include <spdlog/spdlog.h>

namespace mak
{
std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");
}