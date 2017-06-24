#ifndef MAKINA_CORE_RENDERING_FRAMEGRAPH_RENDER_PASS_HPP_
#define MAKINA_CORE_RENDERING_FRAMEGRAPH_RENDER_PASS_HPP_

#include <functional>
#include <vector>

#include <export.hpp>

#include <makina/core/aspects/named.hpp>
#include <makina/core/rendering/framegraph/render_pass_resources.hpp>

namespace mak
{
class render_context;
class resource_builder;
class resource_handle;
class resource_registry;

class MAKINA_EXPORT render_pass_base : public named
{
public:
  render_pass_base(const std::string& name) : named(name)
  {
    
  }
  render_pass_base(const render_pass_base&  that)            = default;
  render_pass_base(      render_pass_base&& temp)            = default;
  virtual ~render_pass_base()                                = default;

  render_pass_base& operator=(const render_pass_base&  that) = default;
  render_pass_base& operator=(      render_pass_base&& temp) = default;

  virtual void setup  (resource_builder& resource_builder)                                           = 0;
  virtual void execute(const render_pass_resources& resources, const render_context* render_context) = 0;

protected:
  std::vector<resource_handle> used_resources_;
};

template<typename data_type>
class MAKINA_EXPORT render_pass : public render_pass_base
{
public:
  render_pass(const std::string&                                                                                name      ,
              const std::function<void(      data_type&,       resource_builder&)>&                             on_setup  ,
              const std::function<void(const data_type&, const render_pass_resources&, const render_context*)>& on_execute)
  : render_pass_base(name      )
  , on_setup_       (on_setup  )
  , on_execute_     (on_execute)
  {
    
  }
  render_pass(const render_pass&  that)            = default;
  render_pass(      render_pass&& temp)            = default;
  virtual ~render_pass()                           = default;
  
  render_pass& operator=(const render_pass&  that) = default;
  render_pass& operator=(      render_pass&& temp) = default;
  
  void setup  (resource_builder& resource_builder)                                           override
  {
    on_setup_  (data_, resource_builder );
  }
  void execute(const render_pass_resources& resources, const render_context* render_context) override
  {
    // TODO: Realize resources.
    on_execute_(data_, resources, render_context);
    // TODO: Destroy resources.
  };

protected:
  data_type                                                                                  data_      ;
  std::function<void(      data_type&,       resource_builder&)>                             on_setup_  ;
  std::function<void(const data_type&, const render_pass_resources&, const render_context*)> on_execute_;
};
}

#endif
