#include <makina/renderer/renderer.hpp>

#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
void renderer::prepare()
{
  compile();
}
void renderer::update (frame_timer::duration delta, scene* scene)
{
  auto camera       = scene->entities<transform, projection> ();
  auto lights       = scene->entities<light>                 ();
  auto mesh_renders = scene->entities<transform, mesh_render>();

  // Backend-independent information:
  // - Who is the main camera? Or do we create a render target for each camera? If so, what is the viewport of each render target?
  // - An array of lights per type.
  // - Vertex, normal, texcoord buffer, index buffers containing data for all meshes.
  // - Used materials and their relationship to the index buffers.

  execute();
}
}
