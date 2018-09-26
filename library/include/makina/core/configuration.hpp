#ifndef MAKINA_CORE_CONFIGURATION_HPP
#define MAKINA_CORE_CONFIGURATION_HPP

#include <nlohmann/json.hpp>

#include <makina/utility/singleton.hpp>
#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT configuration : public singleton<configuration>
{
public:
  explicit configuration  (std::string filepath = "config.json");
  configuration           (const configuration&  that) = delete;
  configuration           (      configuration&& temp) = delete;
  virtual ~configuration  ();
  configuration& operator=(const configuration&  that) = delete;
  configuration& operator=(      configuration&& temp) = delete;

  template<typename type>
  type get(const std::string& key, const type default_value = type())
  {
    if (!json_.count(key)) json_[key] = default_value;
    return json_[key].get<type>();
  }
  template<typename type>
  void set(const std::string& key, const type value)
  {
    json_[key] = value;
  }
  
protected:
  std::string    filepath_;
  nlohmann::json json_    ;
};
}

#endif