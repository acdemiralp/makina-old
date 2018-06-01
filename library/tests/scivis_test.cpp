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
  mak::append_scene(model.scene.get(), engine->scene());
  
  mak::path::add_search_directories({"C:", "C:/dev", "D:", "D:/dev"});
 
  mak::field<float, 3>   field            ;
  mak::transfer_function transfer_function;
  mak::point_cloud       point_cloud      ;
  mak::line_segments     line_segments    ;
  
  // Volume rendering.
  {
    field.load(mak::hdf5_description<float, 3>
    {
      mak::path::find("data/pli/MSA/MSA0309_s0536-0695.h5").string(),
      "Retardation",
      "Spacing"    ,
      mak::selection<float, 3>{{0, 512, 512}, {64, 64, 64}, {1, 1, 1}}
    });

    transfer_function.colors      = {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    transfer_function.opacities   = {0.0001f, 0.0255f};
    transfer_function.value_range = {0.0f   , 255.0f };
    
    auto entity                      = engine->scene()->add_entity();
    auto metadata                    = entity->add_component<mak::metadata>     ();
    auto transform                   = entity->add_component<mak::transform>    (metadata);
    auto volume_render               = entity->add_component<mak::volume_render>();
    volume_render->volume            = &field;
    volume_render->transfer_function = &transfer_function;
  }

  // Point cloud rendering.
  {
    for (auto i = 0; i < 255; i+=8)
      for (auto j = 0; j < 255; j+=8)
        for (auto k = 0; k < 255; k+=8)
        {
          point_cloud.vertices.push_back(glm::vec3  (i, j, k));
          point_cloud.colors  .push_back(glm::u8vec4(i, j, k, 255));
        }
    point_cloud.radius = 4.0f;

    auto entity       = engine->scene()->add_entity();
    auto metadata     = entity->add_component<mak::metadata>    ();
    auto transform    = entity->add_component<mak::transform>   (metadata);
    auto point_render = entity->add_component<mak::point_render>();
    point_render->point_cloud = &point_cloud;
    point_render->material    = model.materials[0].get();
  }

  // Streamline rendering.
  {
    for (auto i = 0; i < 255; i+=8)
      for (auto j = 0; j < 255; j+=8)
        for (auto k = 0; k < 255; k+=8)
        {
          line_segments.vertices.push_back(glm::vec3  (i, j, k));
          line_segments.colors  .push_back(glm::u8vec4(i, j, k, 255));
          if (line_segments.vertices.size() > 1)
          {
            line_segments.indices.push_back(line_segments.vertices.size() - 2);
            line_segments.indices.push_back(line_segments.vertices.size() - 1);
          }
        }
    line_segments.radius = 32.0f;
    
    auto entity      = engine->scene()->add_entity();
    auto metadata    = entity->add_component<mak::metadata>   ();
    auto transform   = entity->add_component<mak::transform>  (metadata);
    auto line_render = entity->add_component<mak::line_render>();
    line_render->line_segments = &line_segments;
    line_render->material      = model.materials[0].get();
  }

  engine->run();
}