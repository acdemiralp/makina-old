#include <makina/makina.hpp>

#include <makina/display/display_system.hpp>
#include <makina/input/input_system.hpp>

namespace mak
{
std::unique_ptr<engine> make_default_engine()
{
  auto   engine = std::make_unique<mak::engine>();
  engine->set_scene                 (std::make_unique<scene>());
  engine->add_system<display_system>();
  engine->add_system<input_system  >();
  return engine;
}
}