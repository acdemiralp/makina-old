#include <catch.hpp>

#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>
#include <makina/makina.hpp>

TEST_CASE("Makina test.", "[makina]")
{
  auto engine = mak::make_default_engine();
  engine->get_system<mak::display_system>()->create_window("Makina", std::array<std::size_t, 2>{32, 32}, std::array<std::size_t, 2>{640, 480});
  engine->get_system<mak::input_system>  ()->on_quit.connect(std::bind(&mak::engine::stop, engine.get()));
  engine->run();
}