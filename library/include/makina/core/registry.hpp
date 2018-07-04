#ifndef MAKINA_CORE_REGISTRY_HPP
#define MAKINA_CORE_REGISTRY_HPP

#include <memory>

#include <ra/registry.hpp>

#include <makina/resources/audio_clip.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/line_segments.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/point_cloud.hpp>
#include <makina/resources/transfer_function.hpp>
#include <makina/export.hpp>

namespace mak
{
using registry_type = ra::registry<audio_clip, image, line_segments, model, point_cloud, transfer_function>;

MAKINA_EXPORT extern std::unique_ptr<registry_type> registry;
}

#endif