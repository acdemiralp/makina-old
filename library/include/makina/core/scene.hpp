#ifndef MAKINA_CORE_SCENE_HPP_
#define MAKINA_CORE_SCENE_HPP_

#include <ec/entity.hpp>
#include <ec/scene.hpp>

#include <makina/core/settings.hpp>

namespace mak
{
using scene = ec::scene<ec::entity<settings::component_types>>;
}

#endif