#ifndef MAKINA_CORE_ASPECTS_NAMED_HPP_
#define MAKINA_CORE_ASPECTS_NAMED_HPP_

#include <string>

#include <export.hpp>

namespace mak
{
class MAKINA_EXPORT named
{
public:
  named()                        = default;
  named(const std::string& name) : name_(name)
  {
    
  }
  named(const named&       that) = default;
  named(      named&&      temp) = default;
  virtual ~named()               = default;

  named& operator=(const named&  that) = default;
  named& operator=(      named&& temp) = default;

  void               set_name(const std::string& name)
  {
    name_ = name;
  }
  const std::string& name    () const
  {
    return name_;
  }
  
protected:
  std::string name_;
};
}

#endif