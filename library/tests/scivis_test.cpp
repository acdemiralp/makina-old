#include <catch.hpp>

#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("SciVis test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (SciVis)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_swap_mode(di::opengl_swap_mode::immediate);
  window->set_resizable(true);
  window->on_resize.connect([&engine] (const std::array<std::size_t, 2>& size)
  {
    engine->scene()->entities<mak::projection>()[0]->component<mak::projection>()->set_perspective(60.0f, float(size[0]) / float(size[1]), {0.3f, 1000.0f});
  });

  mak::opengl::make_default_framegraph(engine.get(), window);

  mak::field<float, 3> field;
  field.load(mak::hdf5_description<float, 3>
  {
    "C:/dev/data/pli/Human/MSA0309_s0536-0695.h5",
    "Retardation",
    "Spacing"    ,
    mak::selection<float, 3>{{0, 0, 0}, {64, 64, 64}, {1, 1, 1}}
  });

  auto entity       = engine->scene()->add_entity();
  auto metadata     = entity->add_component<mak::metadata>    ();
  auto transform    = entity->add_component<mak::transform>   ();
  auto field_render = entity->add_component<mak::field_render>();
  // field_render->field = &field;

  engine->run();
}