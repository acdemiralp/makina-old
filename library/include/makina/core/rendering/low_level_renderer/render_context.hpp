#ifndef MAKINA_CORE_RENDERING_LOW_LEVEL_RENDERER_RENDER_CONTEXT_HPP_
#define MAKINA_CORE_RENDERING_LOW_LEVEL_RENDERER_RENDER_CONTEXT_HPP_

#include <export.hpp>

namespace mak
{
class MAKINA_EXPORT render_context
{
public:
  render_context()                            = default;
  render_context(const render_context&  that) = default;
  render_context(      render_context&& temp) = default;
  virtual ~render_context()                   = default;

  render_context& operator=(const render_context&  that) = default;
  render_context& operator=(      render_context&& temp) = default;

protected:
  
};
}

#endif
