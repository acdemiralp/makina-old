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

  // Backend-independent render information:
  // - 5 buffers for vertices, normals, texcoords, instance attributes, indices of all meshes.
  // - 1 buffer  for lights.
  // - 1 buffer  for materials.
  // - 1 buffer  for transform matrices and index spans (per mesh   instance).
  // - 1 buffer  for view and projection matrices       (per camera instance).
  // - N images  for each channel of each material.
  // - 1 index   for the main camera.
  // - 1 retained render target (or 2 for VR) for the backbuffer.

  // Notes:
  // - Any other resources are created and managed by the render tasks.
  // - Render task writing to the backbuffer controls what goes to the screen.

  execute();
}
}
