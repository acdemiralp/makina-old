#ifndef MAKINA_AUDIO_FMOD_CONTEXT_HPP_
#define MAKINA_AUDIO_FMOD_CONTEXT_HPP_

#include <fmod.hpp>

#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT FMOD::System* fmod_context();
}

#endif