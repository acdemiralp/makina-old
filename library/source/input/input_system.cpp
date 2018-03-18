#include <makina/input/input_system.hpp>

#include <makina/input/controller.hpp>

namespace mak
{
void input_system::update(frame_timer::duration delta, scene* scene)
{
  tick();

  auto entities = scene->entities<mak::controller, mak::transform>();
  for(auto& entity : entities)
  {
    auto controller = entity->component<mak::controller>();
    auto transform  = entity->component<mak::transform> ();
    if (std::find(controller_cache_.begin(), controller_cache_.end(), controller) == controller_cache_.end())
    {
      if(controller->on_key_press    ) on_key_press       .connect([=] (di::key                    key   ) { controller->on_key_press    (transform, key   ); });
      if(controller->on_key_release  ) on_key_release     .connect([=] (di::key                    key   ) { controller->on_key_release  (transform, key   ); });
      if(controller->on_mouse_press  ) on_mouse_press     .connect([=] (std::size_t                button) { controller->on_mouse_press  (transform, button); });
      if(controller->on_mouse_release) on_mouse_release   .connect([=] (std::size_t                button) { controller->on_mouse_release(transform, button); });
      if(controller->on_mouse_move   ) on_mouse_move_delta.connect([=] (std::array<std::size_t, 2> delta ) { controller->on_mouse_move   (transform, delta ); });
      if(controller->on_mouse_wheel  ) on_mouse_wheel     .connect([=] (std::array<std::size_t, 2> delta ) { controller->on_mouse_wheel  (transform, delta ); });
      controller_cache_.push_back(controller);
    }
  }
}
}
