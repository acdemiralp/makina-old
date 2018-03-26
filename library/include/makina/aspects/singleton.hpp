#ifndef MAKINA_ASPECTS_SINGLETON_HPP_
#define MAKINA_ASPECTS_SINGLETON_HPP_

#include <memory>
#include <mutex>
#include <utility>

namespace mak
{
// Note: Do not use a singleton unless you are able to justify that 
// it is the only optimal solution to the problem you are facing.
template<typename derived>
class singleton
{
public:
  template<typename... argument_types>
  static derived& get(argument_types&&... arguments)
  {
    static std::once_flag once;
    std::call_once(once, [ ] (argument_types&&... arguments)
    {
      instance_ = std::make_unique<derived>(std::forward<argument_types>(arguments)...);
    });
    return *instance_.get();
  }

protected:
  explicit singleton  ()                       = default;
  singleton           (const singleton&  that) = delete ;
  singleton           (      singleton&& temp) = default;
  virtual ~singleton  ()                       = default;
  singleton& operator=(const singleton&  that) = delete ;
  singleton& operator=(      singleton&& temp) = default;

private:
  static std::unique_ptr<derived> instance_;
};
}

#endif