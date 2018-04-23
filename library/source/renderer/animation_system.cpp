#include <makina/renderer/animation_system.hpp>

#include <makina/renderer/animator.hpp>

namespace mak
{
void animation_system::prepare(scene* scene)
{

}
void animation_system::update(frame_timer::duration delta, scene* scene)
{
  for(auto& entity : scene->entities<mak::animator>())
  {
    auto animator = entity->component<mak::animator>();
    if (!animator->play) continue;

    animator->time += delta.count();
    if (animator->time >= animator->clip->length)
    {
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::once )
      {
        animator->time = 0.0f;
        animator->play = false;
      }
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::clamp)
        animator->time  = animator->clip->length;
      if (animator->clip->wrap_mode == animation_clip::wrap_mode::loop )
        animator->time -= animator->clip->length;
    }

    //std::function<void(transform*)> recursive_apply = [&] (transform* transform)
    //{
    //  if (animator->clip->translation_curves.count(transform->metadata()->name))
    //    transform->set_translation(animator->clip->translation_curves[transform->metadata()->name].lerp (animator->time), true);
    //  if (animator->clip->rotation_curves   .count(transform->metadata()->name))
    //    transform->set_rotation   (animator->clip->rotation_curves   [transform->metadata()->name].slerp(animator->time), true);
    //  if (animator->clip->scaling_curves    .count(transform->metadata()->name))
    //    transform->set_scale      (animator->clip->scaling_curves    [transform->metadata()->name].lerp (animator->time), true);
    //
    //  for (auto child : transform->children())
    //    recursive_apply(transform);
    //};
    //recursive_apply(animator->root_transform);
  }
}
}
