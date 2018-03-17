#ifndef MAKINA_RENDERER_BACKEND_PARAMETER_MAP_HPP_
#define MAKINA_RENDERER_BACKEND_PARAMETER_MAP_HPP_

#include <map>

#include <boost/lexical_cast.hpp>
#include <fg/resource.hpp>

#include <makina/export.hpp>

namespace mak
{
// Utility resource for passing data between render tasks live, during traversal.
class MAKINA_EXPORT parameter_map
{
public:
  class MAKINA_EXPORT description { };

  template<typename type>
  void set(const std::string& key, const type& value)
  {
    map_[key] = boost::lexical_cast<std::string>(value);
  }
  template<typename type>
  type get(const std::string& key) const
  {
    return map_.count(key) ? boost::lexical_cast<type>(map_.at(key)) : type();
  }

protected:
  std::map<std::string, std::string> map_;
};

using parameter_map_resource = fg::resource<parameter_map::description, parameter_map>;
}

template<>
inline std::unique_ptr<mak::parameter_map> fg::realize(const mak::parameter_map::description& description)
{
  return std::make_unique<mak::parameter_map>();
}

#endif