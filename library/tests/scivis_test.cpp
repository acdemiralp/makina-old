#include <catch.hpp>

#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("SciVis test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  engine->remove_system<mak::vr_system>();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (SciVis)", 
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
  engine->scene()->copy_entity(model.scene->entities()[1]);
  
  mak::field<float, 3> field;
  field.load(mak::hdf5_description<float, 3>
  {
    "D:/data/pli/MSA/MSA0309_s0536-0695.h5",
    "Retardation",
    "Spacing"    ,
    mak::selection<float, 3>{{0, 512, 512}, {64, 64, 64}, {1, 1, 1}}
  });

  mak::point_cloud point_cloud;
  for (auto i = 0; i < 255; ++i)
    for (auto j = 0; j < 255; ++j)
      for (auto k = 0; k < 255; ++k)
      {
        point_cloud.vertices.push_back(glm::vec3  (i, j, k));
        point_cloud.colors  .push_back(glm::u8vec4(i, j, k, 255));
      }

  mak::line_segments line_segments;
  for (auto i = 0; i < 255; ++i)
    for (auto j = 0; j < 255; ++j)
      for (auto k = 0; k < 255; ++k)
      {
        line_segments.vertices.push_back(glm::vec3  (i, j, k));
        line_segments.colors  .push_back(glm::u8vec4(i, j, k, 255));
        if (k > 0)
        {
          line_segments.indices.push_back(k - 1);
          line_segments.indices.push_back(k);
        }
      }

  auto entity       = engine->scene()->add_entity();
  auto metadata     = entity->add_component<mak::metadata>    ();
  auto transform    = entity->add_component<mak::transform>   (metadata);
  auto field_render = entity->add_component<mak::field_render>();
  // field_render->field = &field;

  engine->run();
}