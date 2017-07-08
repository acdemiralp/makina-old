#ifndef MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_REGISTRY_HPP_
#define MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_REGISTRY_HPP_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include <makina/core/rendering/resources/resource.hpp>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT resource_registry
{
public:
  resource_registry()                                          = default;
  resource_registry(const resource_registry&  that)            = delete ;
  resource_registry(      resource_registry&& temp)            = default;
  virtual ~resource_registry()                                 = default;

  resource_registry& operator=(const resource_registry&  that) = delete ;
  resource_registry& operator=(      resource_registry&& temp) = default;

  template<typename resource_type, typename... arguments_type>
  resource_type* add_resource    (arguments_type&&... arguments   )
  {
    auto& resource_vector = resources_[typeid(resource_type)];
    resource_vector.emplace_back(new resource_type(arguments...));
    return static_cast<resource_type*>(resource_vector.back().get());
  }
  template<typename resource_type>
  resource_type* get_resource    (std::size_t         index       ) const
  {
    return resources_.at(typeid(resource_type))[index].get();
  }
  template<typename resource_type>
  void           remove_resource (std::size_t         index       )
  {
    auto& resource_vector = resources_[typeid(resource_type)];
    resource_vector.erase(resource_vector.begin(), index);
  }
  template<typename resource_type>
  void           remove_resource (resource_type*      resource_ptr)
  {
    auto& resource_vector = resources_[typeid(resource_type)];
    resource_vector.erase(std::remove(resource_vector.begin(), resource_vector.end(), 
    [] (const std::unique_ptr<resource>& iteratee)
    {
      return resource_ptr == iteratee.get();
    }));
  }

  template<typename resource_type>  
  std::vector<resource_type*> get_resource_vector  () const
  {
    const auto& resource_vector = resources_.at(typeid(resource_type));
    std::vector<resource_type*> transform_resource_vector(resource_vector.size());
    std::transform(resource_vector.begin(), resource_vector.end(), transform_resource_vector.begin(),
    [] (const std::unique_ptr<resource>& iteratee)
    {
      return static_cast<resource_type*>(iteratee.get());
    });
    return transform_resource_vector;
  }
  template<typename resource_type>  
  void                        clear_resource_vector()
  {
    resources_[typeid(resource_type)].clear();
  }
  void                        clear                ()
  {
    resources_.clear();
  }

protected:
  std::unordered_map<std::type_index, std::vector<std::unique_ptr<resource>>> resources_;
};
}

#endif
