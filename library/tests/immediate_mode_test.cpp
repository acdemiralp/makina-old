#include <catch.hpp>

#include <makina/renderer/backend/opengl/render_tasks.hpp>
#include <makina/api.hpp>

TEST_CASE("Immediate mode test.", "[makina]")
{
  auto engine = mak::make_default_engine();

  const auto display_system = engine->get_system<mak::display_system>();
  const auto window         = display_system->create_opengl_window(
    "Makina (Immediate Mode)", 
    std::array<std::size_t, 2>{32 , 32 }, 
    std::array<std::size_t, 2>{800, 600}, 
    di::opengl_context_settings{di::opengl_profile::core, 4, 5});
  window->set_resizable(true);
  window->on_resize.connect([&engine] (const std::array<std::size_t, 2>& size)
  {
    engine->scene()->entities<mak::projection>()[0]->component<mak::projection>()->set_perspective(60.0f, float(size[0]) / float(size[1]), {0.3f, 1000.0f});
  });

  mak::opengl::make_default_framegraph(engine.get(), window);
  
  auto entity    = engine->scene()->add_entity();
  auto behaviors = entity->add_component<mak::behaviors>();
  behaviors->push_back(std::make_shared<mak::behavior>(
  [ ] (                                     mak::scene* scene, mak::entity* entity)
  {

  },
  [ ] (mak::frame_timer::duration duration, mak::scene* scene, mak::entity* entity)
  {
    mak::random_number_generator<std::uniform_real_distribution<float>> rng(0.0f, 10.0f);

    ImGui::SetCurrentContext(mak::imgui_context());
    ImGui::Begin            ("FPS");
    ImGui::Text             ("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End              ();

    Im3d::SetContext        (mak::im3d_context());
    Im3d::PushDrawState     ();
    Im3d::SetSize           (10.0f);
    Im3d::BeginPoints       ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Magenta);
      Im3d::Vertex          (rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Yellow );
      Im3d::Vertex          (rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Cyan   );
    }
    
    Im3d::End               ();
    Im3d::BeginLineLoop     ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (10.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Magenta);
      Im3d::Vertex          (10.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Yellow );
      Im3d::Vertex          (10.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Cyan   );
    }
    
    Im3d::End               ();
    Im3d::BeginTriangles    ();
    
    for (auto i = 0; i < 100; ++i)
    {
      Im3d::Vertex          (20.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Magenta);
      Im3d::Vertex          (20.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Yellow );
      Im3d::Vertex          (20.0f + rng.generate(), rng.generate(), rng.generate(), Im3d::Color_Cyan   );
    }
    
    Im3d::End               ();
    Im3d::PopDrawState      ();
  }));

  engine->run();
}