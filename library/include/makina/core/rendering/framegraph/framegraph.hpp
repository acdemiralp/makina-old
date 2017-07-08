#ifndef MAKINA_CORE_RENDERING_FRAMEGRAPH_FRAMEGRAPH_HPP_
#define MAKINA_CORE_RENDERING_FRAMEGRAPH_FRAMEGRAPH_HPP_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <makina_export.hpp>

namespace mak
{
class render_context;
template<typename pass_data_t>
class render_pass;
class render_pass_base;
class render_pass_resources;
class resource_builder;
class resource_registry;

class MAKINA_EXPORT frame_graph
{
public:
  frame_graph(
    resource_registry&    resource_registry,
    resource_builder&     resource_builder ,
    const render_context* render_context   );
  frame_graph(const frame_graph&  that)            = default;
  frame_graph(      frame_graph&& temp)            = default;
  virtual ~frame_graph()                           = default;
                                                   
  frame_graph& operator=(const frame_graph&  that) = default;
  frame_graph& operator=(      frame_graph&& temp) = default;

  template<typename pass_data_t>
  render_pass<pass_data_t>* add_render_pass(
    const std::string&                                                                                  name      ,
    const std::function<void(      pass_data_t&,       resource_builder&)>&                             on_setup  ,
    const std::function<void(const pass_data_t&, const render_pass_resources&, const render_context*)>& on_execute);

  void traverse() const;

protected:
  resource_registry&                             resource_registry_;
  resource_builder &                             resource_builder_ ;
  const render_context*                          render_context_   ;
  std::vector<std::unique_ptr<render_pass_base>> render_passes_    ;
};

template <typename pass_data_t>
render_pass<pass_data_t>* frame_graph::add_render_pass(
  const std::string&                                                                                  name      , 
  const std::function<void(      pass_data_t&,       resource_builder&)>&                             on_setup  , 
  const std::function<void(const pass_data_t&, const render_pass_resources&, const render_context*)>& on_execute)
{
  render_passes_.emplace_back(new render_pass<pass_data_t>(name, on_setup, on_execute));
  return static_cast<render_pass<pass_data_t>*>(render_passes_.back().get());
}
}

#endif
