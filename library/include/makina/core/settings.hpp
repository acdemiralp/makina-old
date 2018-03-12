#ifndef MAKINA_CORE_SETTINGS_HPP_
#define MAKINA_CORE_SETTINGS_HPP_

#include <ratio>

#include <ec/entity.hpp>
#include <ra/registry.hpp>

#include <makina/renderer/light.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/model.hpp>

namespace mak
{
// Compile-time settings for the engine.
namespace settings
{
  using temporal_type   = float;
  using temporal_period = std::milli;
  using entity_type     = ec::entity<light, mesh_render, projection, transform>;
  using registry_type   = ra::registry<image, model>;
}
}

#endif