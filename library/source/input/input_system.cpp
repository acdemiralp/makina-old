#include <makina/input/input_system.hpp>

#include <makina/input/controller.hpp>

namespace mak
{
input_system::input_system()
{
  on_key_press    .connect([&] (di::key     key   ) { key_states_   [key]    = true ; });
  on_key_release  .connect([&] (di::key     key   ) { key_states_   [key]    = false; });
  on_mouse_press  .connect([&] (std::size_t button) { button_states_[button] = true ; });
  on_mouse_release.connect([&] (std::size_t button) { button_states_[button] = false; });
}

void input_system::update(frame_timer::duration delta, scene* scene)
{
  tick();

  for (auto& state : key_states_   )
    if (state.second) on_key_down  (state.first);
  for (auto& state : button_states_)
    if (state.second) on_mouse_down(state.first);

  auto entities = scene->entities<mak::controller, mak::transform>();
  for(auto& entity : entities)
  {
    auto controller = entity->component<mak::controller>();
    auto transform  = entity->component<mak::transform> ();
    if (std::find(controller_cache_.begin(), controller_cache_.end(), controller) == controller_cache_.end())
    {
      if(controller->on_key_press    ) on_key_press       .connect([=] (di::key                    key   ) { controller->on_key_press    (transform, delta, key   ); });
      if(controller->on_key_down     ) on_key_down        .connect([=] (di::key                    key   ) { controller->on_key_down     (transform, delta, key   ); });
      if(controller->on_key_release  ) on_key_release     .connect([=] (di::key                    key   ) { controller->on_key_release  (transform, delta, key   ); });
      if(controller->on_mouse_press  ) on_mouse_press     .connect([=] (std::size_t                button) { controller->on_mouse_press  (transform, delta, button); });
      if(controller->on_mouse_down   ) on_mouse_down      .connect([=] (std::size_t                button) { controller->on_mouse_down   (transform, delta, button); });
      if(controller->on_mouse_release) on_mouse_release   .connect([=] (std::size_t                button) { controller->on_mouse_release(transform, delta, button); });
      if(controller->on_mouse_move   ) on_mouse_move_delta.connect([=] (std::array<std::size_t, 2> value ) { controller->on_mouse_move   (transform, delta, value ); });
      if(controller->on_mouse_wheel  ) on_mouse_wheel     .connect([=] (std::array<std::size_t, 2> value ) { controller->on_mouse_wheel  (transform, delta, value ); });
      controller_cache_.push_back(controller);
    }
  }
}
}
