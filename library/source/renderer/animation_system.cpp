#include <makina/renderer/animation_system.hpp>

#include <tbb/tbb.h>

#include <makina/renderer/animator.hpp>

namespace mak
{
void animation_system::prepare(scene* scene)
{

}
void animation_system::update (const frame_timer::duration delta, scene* scene)
{
  for(auto& entity : scene->entities<mak::animator>())
  {
    auto animator = entity->component<mak::animator>();
    if (!animator->play) continue;

    animator->time += delta;
    if (animator->time >= animator->clip->length)
    {
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::once )
      {
        animator->time = frame_timer::duration();
        animator->play = false;
      }
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::clamp)
        animator->time  = animator->clip->length;
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::loop )
        animator->time -= animator->clip->length;
    }

    std::function<void(transform*)> recursive_apply = [&] (transform* transform)
    {
      transform->set_auto_commit(false);
      if (animator->clip->translation_curves.count(transform->metadata()->name))
        transform->set_translation(animator->clip->translation_curves[transform->metadata()->name].lerp (animator->time));
      if (animator->clip->rotation_curves   .count(transform->metadata()->name))
        transform->set_rotation   (animator->clip->rotation_curves   [transform->metadata()->name].slerp(animator->time));
      if (animator->clip->scaling_curves    .count(transform->metadata()->name))
        transform->set_scale      (animator->clip->scaling_curves    [transform->metadata()->name].lerp (animator->time));
      transform->commit         ();
      transform->set_auto_commit(true );
    
      tbb::parallel_for(std::size_t(0), transform->children().size(), std::size_t(1), [=] (std::size_t i)
      {
        recursive_apply(transform->children()[i]);
      });
    };
    recursive_apply(animator->root_transform);
  }
}
}
