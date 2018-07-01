#include <makina/audio/audio_system.hpp>

#include <mutex>

#include <makina/audio/fmod_context.hpp>
#include <makina/core/logger.hpp>
#include <makina/renderer/transform.hpp>

namespace mak
{
void audio_system::prepare(scene* scene)
{
  if (scene->entities<audio_listener>().size() > 1)
    logger->warn("Multiple audio listeners in the scene! Only the first one will be utilized.");
  
  for (auto source_entity : scene->entities<audio_source>())
  {
    auto source = source_entity->component<audio_source>();
    if (source->autoplay())
      source->play();
  }
}
void audio_system::update (frame_timer::duration delta, scene* scene)
{
  auto listener_entities = scene->entities<audio_listener>();
  if  (listener_entities.size() == 0) return;
  
  const auto listener_transform   = listener_entities[0]->component<transform>();
  const auto listener_rigidbody   = listener_entities[0]->component<rigidbody>();
  auto       listener_translation = listener_transform->translation(true);
  auto       listener_forward     = listener_transform->forward    (true);
  auto       listener_up          = listener_transform->up         (true);
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
    const auto source_transform   = source_entities->component<transform>   ();
    const auto source_rigidbody   = source_entities->component<rigidbody>   ();
    auto       source             = source_entities->component<audio_source>();
    auto       source_translation = source_transform->translation(true);
    glm::vec3  source_velocity    ;
    if (source_rigidbody)
      source_velocity = source_rigidbody->velocity();
    if(source->native_)
      source->native_->set3DAttributes(
        reinterpret_cast<FMOD_VECTOR*>(&source_translation),
        reinterpret_cast<FMOD_VECTOR*>(&source_velocity   ));
  }

  fmod_context()->update();
}
}
