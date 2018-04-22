#ifndef MAKINA_CORE_METADATA_HPP_
#define MAKINA_CORE_METADATA_HPP_

#include <algorithm>
#include <string>
#include <vector>

namespace mak
{
struct metadata
{
  bool contains_tag(const std::string& tag)
  {
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
  }

  std::string              name;
  std::vector<std::string> tags;
};
}

#endif