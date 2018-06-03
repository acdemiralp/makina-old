#include <catch.hpp>

#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("SciVis test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  engine->remove_system<mak::vr_system>();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (B-Splines)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_resizable(true);
  window->on_resize.connect([&engine] (const std::array<std::size_t, 2>& size)
  {
    engine->scene()->entities<mak::projection>()[0]->component<mak::projection>()->set_perspective(60.0f, float(size[0]) / float(size[1]), {0.3f, 1000.0f});
  });

  mak::opengl::make_default_framegraph(engine.get(), window);
  
  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  model.load(mak::model::description{std::string("data/model/cube/cube.obj"), true});
  mak::append_scene(model.scene.get(), engine->scene());
  
  mak::b_spline b_spline(std::vector<std::tuple<std::vector<double>, double>>{
    {{0.0, 0.0}, 0.0},
    {{0.0, 1.0}, 0.0},
    {{0.0, 2.0}, 0.0},
    {{1.0, 0.0}, 0.0},
    {{1.0, 1.0}, 5.0},
    {{1.0, 2.0}, 0.0},
    {{2.0, 0.0}, 0.0},
    {{2.0, 1.0}, 0.0},
    {{2.0, 2.0}, 0.0}}, 2);
  
  auto point_cloud         = b_spline.to_point_cloud  ({0.0, 0.0}, {2.0, 2.0}, {100, 100});
  auto line_segments       = b_spline.to_line_segments({0.0, 0.0}, {2.0, 2.0}, {100, 100});
  auto control_point_cloud = b_spline.control_points_to_point_cloud();
  
  { 
    point_cloud->radius = 2.0f;

    auto entity       = engine->scene()->add_entity();
    auto metadata     = entity->add_component<mak::metadata>    ();
    auto transform    = entity->add_component<mak::transform>   (metadata);
    auto point_render = entity->add_component<mak::point_render>();
    point_render->point_cloud = point_cloud.get();
    point_render->material    = model.materials[0].get();
  }

  {
    line_segments->radius = 4.0f;

    auto entity      = engine->scene()->add_entity();
    auto metadata    = entity->add_component<mak::metadata>   ();
    auto transform   = entity->add_component<mak::transform>  (metadata);
    auto line_render = entity->add_component<mak::line_render>();
    line_render->line_segments = line_segments.get();
    line_render->material      = model.materials[0].get();
  }

  { 
    control_point_cloud->radius = 10.0f;

    auto entity       = engine->scene()->add_entity();
    auto metadata     = entity->add_component<mak::metadata>    ();
    auto transform    = entity->add_component<mak::transform>   (metadata);
    auto point_render = entity->add_component<mak::point_render>();
    point_render->point_cloud = control_point_cloud.get();
    point_render->material    = model.materials[0].get();
  }


  engine->run();
}