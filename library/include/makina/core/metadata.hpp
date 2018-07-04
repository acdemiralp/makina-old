#ifndef MAKINA_CORE_METADATA_HPP
#define MAKINA_CORE_METADATA_HPP

#include <algorithm>
#include <string>
#include <vector>

#include <makina/core/scene_fwd.hpp>

namespace mak
{
struct metadata
{
  bool contains_tag(const std::string& tag)
  {
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
  }
  
  entity*                  entity ;
  std::string              name   ;
  std::vector<std::string> tags   ;
  bool                     active = true;
};
}

#endif