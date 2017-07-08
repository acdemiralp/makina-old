#ifndef MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_BUILDER_HPP_
#define MAKINA_CORE_RENDERING_RESOURCES_RESOURCE_BUILDER_HPP_

#include <bitset>
#include <cstddef>
#include <typeindex>
#include <unordered_map>

#include <makina/core/rendering/resources/resource_registry.hpp>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT resource_builder
{
public:
  resource_builder(resource_registry* resource_registry) : resource_registry_(resource_registry)
  {
    
  }
  resource_builder(const resource_builder&  that)            = default;
  resource_builder(      resource_builder&& temp)            = default;
  virtual ~resource_builder()                                = default;

  resource_builder& operator=(const resource_builder&  that) = default;
  resource_builder& operator=(      resource_builder&& temp) = default;
  
  // TODO: Rework these.
  template<typename resource_type>
  std::size_t create(const typename resource_type::description& description)
  {
    auto& resource_description_vector = resource_descriptions_[typeid(resource_type)];
    resource_description_vector.emplace_back(new typename resource_type::description(description));
    return resource_description_vector.size() - 1;
  }
  //template<typename resource_type>
  //std::size_t read  (std::size_t index)
  //{
  //  auto&  resource_description_vector = resource_descriptions_[typeid(resource_type)];
  //
  //  return resource_description_vector.size() - 1;
  //}
  //template<typename resource_type>
  //std::size_t write (std::size_t index)
  //{
  //  auto&  resource_description_vector = resource_descriptions_[typeid(resource_type)];
  //  // TODO: Alias resource.
  //  return resource_description_vector.size() - 1;
  //}
  //
  void realize()
  {
    // Go through the resource descriptions and realize it in the resource_registry.
    //for(auto& resource_descriptions : resource_descriptions_)
    //  for(auto& resource_description : resource_descriptions.second)
    //    resource_registry_->add_resource<decltype(*resource_description)>();
  }

protected:
  std::unordered_map<std::type_index, std::vector<std::unique_ptr<resource::description>>> resource_descriptions_;
  resource_registry*                                                                       resource_registry_    ;
};
}

#endif
