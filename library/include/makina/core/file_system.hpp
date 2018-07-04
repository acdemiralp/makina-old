#ifndef MAKINA_CORE_FILE_SYSTEM_HPP
#define MAKINA_CORE_FILE_SYSTEM_HPP

#include <filesystem>
#include <vector>

#include <makina/export.hpp>

namespace mak
{
namespace path
{
MAKINA_EXPORT extern std::vector<std::experimental::filesystem::path> search_directories;

MAKINA_EXPORT void                                add_search_directories   (const std::vector<std::experimental::filesystem::path>& directories  );
MAKINA_EXPORT void                                remove_search_directories(const std::vector<std::experimental::filesystem::path>& directories  );
MAKINA_EXPORT std::experimental::filesystem::path find                     (const std::experimental::filesystem::path&              relative_path);
}
}

#endif