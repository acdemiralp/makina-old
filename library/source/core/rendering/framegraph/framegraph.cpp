#include <makina/core/rendering/framegraph/framegraph.hpp>

#include <makina/core/rendering/framegraph/render_pass.hpp>
#include <makina/core/rendering/resources/resource_builder.hpp>
#include <makina/core/rendering/resources/resource_registry.hpp>

namespace mak
{
frame_graph::frame_graph(
  resource_registry&    resource_registry, 
  resource_builder&     resource_builder , 
  const render_context* render_context   )
  : resource_registry_(resource_registry)
  , resource_builder_ (resource_builder )
  , render_context_   (render_context   )
{

}

void frame_graph::traverse() const
{
  // TODO: Cull disconnected render passes.

  for (auto& render_pass : render_passes_)
    render_pass->setup(resource_builder_);
  for (auto& render_pass : render_passes_)
  {
    // Realize resources.
    render_pass->execute(render_pass_resources(), render_context_);
    // Destroy resources.
  }
}
}
