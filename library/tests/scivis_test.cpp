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

  auto entity       = engine->scene()->add_entity();
  auto metadata     = entity->add_component<mak::metadata>    ();
  auto transform    = entity->add_component<mak::transform>   (metadata);
  auto behaviors    = entity->add_component<mak::behaviors>   ();
  auto field_render = entity->add_component<mak::field_render>();
  behaviors->push_back(std::make_shared<mak::behavior>(
  [ ] (                                     mak::scene* scene, mak::entity* entity)
  {

  },
  [ ] (mak::frame_timer::duration duration, mak::scene* scene, mak::entity* entity)
  {
    ImGui::SetCurrentContext(mak::imgui_context());
    ImGui::Begin            ("FPS");
    ImGui::Text             ("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End              ();

    Im3d::SetContext        (mak::im3d_context());
    Im3d::PushDrawState     ();
    Im3d::SetSize           (2.0f);
    Im3d::BeginLineLoop     ();
    Im3d::Vertex            (0.0f, 0.0f, 0.0f, Im3d::Color_Magenta);
    Im3d::Vertex            (1.0f, 1.0f, 0.0f, Im3d::Color_Yellow);
    Im3d::Vertex            (2.0f, 2.0f, 0.0f, Im3d::Color_Cyan);
    Im3d::End               ();
    Im3d::PopDrawState      ();
  }));
  // field_render->field = &field;

  engine->run();
}