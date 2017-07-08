#ifndef MAKINA_CORE_RENDERING_FRAMEGRAPH_RENDER_PASS_RESOURCES_HPP_
#define MAKINA_CORE_RENDERING_FRAMEGRAPH_RENDER_PASS_RESOURCES_HPP_

#include <vector>

#include <makina_export.hpp>

#include <makina/core/rendering/resources/resource.hpp>

namespace mak
{
class MAKINA_EXPORT render_pass_resources
{
public:
  render_pass_resources()                                              = default;
  render_pass_resources(const render_pass_resources&  that)            = default;
  render_pass_resources(      render_pass_resources&& temp)            = default;
  virtual ~render_pass_resources()                                     = default;

  render_pass_resources& operator=(const render_pass_resources&  that) = default;
  render_pass_resources& operator=(      render_pass_resources&& temp) = default;

protected:
  std::vector<resource*> resources_;
};
}

#endif
