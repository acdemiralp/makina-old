#ifndef MAKINA_AUDIO_FMOD_CONTEXT_HPP
#define MAKINA_AUDIO_FMOD_CONTEXT_HPP

#include <fmod.hpp>

#include <makina/export.hpp>

namespace mak
{
MAKINA_EXPORT FMOD::System* fmod_context();
}

#endif