#include <makina/audio/audio_system.hpp>

#include <mutex>

#include <makina/audio/fmod_context.hpp>
#include <makina/core/logger.hpp>

namespace mak
{
audio_system::audio_system()
{

}

void audio_system::prepare(scene* scene)
{
  if (scene->entities<audio_listener>().size() > 1)
    logger->warn("Multiple audio listeners in the scene! Only the first one will be utilized.");
}
void audio_system::update (frame_timer::duration delta, scene* scene)
{
  auto listener_entities = scene->entities<audio_listener>();
  if  (listener_entities.size() != 1) return;
  
  const auto listener_transform   = listener_entities[0]->component<transform>();
  const auto listener_rigidbody   = listener_entities[0]->component<rigidbody>();
  auto       listener_translation = listener_transform->translation();
  auto       listener_forward     = listener_transform->forward    ();
  auto       listener_up          = listener_transform->up         ();
  glm::vec3  listener_velocity    ;
  if (listener_rigidbody)
    listener_velocity = listener_rigidbody->velocity();
  fmod_context()->set3DListenerAttributes(
    0,
    reinterpret_cast<FMOD_VECTOR*>(&listener_translation),
    reinterpret_cast<FMOD_VECTOR*>(&listener_velocity   ),
    reinterpret_cast<FMOD_VECTOR*>(&listener_forward    ),
    reinterpret_cast<FMOD_VECTOR*>(&listener_up         ));

  for (auto source_entities : scene->entities<audio_source>())
  {
    auto source_transform = source_entities[0].component<transform>   ();
    auto source           = source_entities[0].component<audio_source>();

    // TODO!
  }

  fmod_context()->update();
}
}
