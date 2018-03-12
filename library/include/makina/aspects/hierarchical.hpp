#ifndef MAKINA_ASPECTS_HIERARCHICAL_HPP_
#define MAKINA_ASPECTS_HIERARCHICAL_HPP_

#include <algorithm>
#include <vector>

#include <makina/export.hpp>

namespace mak
{
template<typename derived>
class MAKINA_EXPORT hierarchical
{
public:
  virtual ~hierarchical() = default;

  void                         set_parent(derived* parent)
  {
    if (parent_)
      parent_->children_.erase(std::remove(parent_->children_.begin(), parent_->children_.end(), static_cast<derived*>(this)), parent_->children_.end());
    
    parent_ = parent;

    if (parent_)
      parent_->children_.push_back(static_cast<derived*>(this));
  }
  derived*                     parent    ()
  {
    return parent_  ;
  }
  const std::vector<derived*>& children  () const
  {
    return children_;
  }

protected:
  derived*              parent_  = nullptr;
  std::vector<derived*> children_;
};
}

#endif