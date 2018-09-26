#include <makina/core/configuration.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>

namespace mak
{
configuration::configuration (std::string filepath) : filepath_(std::move(filepath))
{
  if (!std::experimental::filesystem::exists(filepath_)) return;
  std::ifstream stream(filepath_);
  stream >> json_;
}
configuration::~configuration()
{
  std::ofstream stream(filepath_);
  stream << std::setw(2) << json_ << std::endl;
}
}
