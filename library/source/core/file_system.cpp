#include <makina/core/file_system.hpp>

#include <makina/core/logger.hpp>

namespace mak
{
namespace path
{
std::vector<std::experimental::filesystem::path> search_directories;

void                                add_search_directories   (const std::vector<std::experimental::filesystem::path>& directories  )
{
  search_directories.insert(search_directories.end(), directories.begin(), directories.end());
}
void                                remove_search_directories(const std::vector<std::experimental::filesystem::path>& directories  )
{
  search_directories.erase(std::remove_if(
    search_directories.begin(),
    search_directories.end  (),
    [&](auto iteratee)
    {
      return std::find(directories.begin(), directories.end(), iteratee) != directories.end();
    }),
    search_directories.end  ());
}
std::experimental::filesystem::path find                     (const std::experimental::filesystem::path&              relative_path)
{
  if (exists(relative_path))
    return relative_path;
  for (auto& directory : search_directories)
    if (exists(directory / relative_path))
      return directory / relative_path;
  logger->error("Failed to find path: {}.", relative_path.string());
  return {};
}
}
}
