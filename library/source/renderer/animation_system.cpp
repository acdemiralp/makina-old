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

    // TODO: LERP/SLERP curves and apply to corresponding children of the animator entity.
    for (auto curve : animator->clip->translation_curves)
    {
      
    }
    for (auto curve : animator->clip->rotation_curves   )
    {

    }
    for (auto curve : animator->clip->scaling_curves    )
    {

    }
  }
}
}