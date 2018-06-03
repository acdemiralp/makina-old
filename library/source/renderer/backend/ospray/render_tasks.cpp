#include <makina/renderer/backend/ospray/render_tasks.hpp>

#include <ospray/ospray_cpp.h>

#include <makina/core/engine.hpp>
#include <makina/renderer/backend/opengl/framebuffer.hpp>

namespace mak
{
namespace ospray
{
void make_default_framegraph(engine* engine, di::window* window, bool pathtracing)
{
  int          argc = 0;
  const char** argv = nullptr;
  ospInit(&argc, argv);
}
}
}