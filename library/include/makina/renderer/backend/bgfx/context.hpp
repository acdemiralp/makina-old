#ifndef MAKINA_RENDERER_BACKEND_BGFX_CONTEXT_HPP
#define MAKINA_RENDERER_BACKEND_BGFX_CONTEXT_HPP

#include <bgfx/bgfx.h>
#include <di/systems/display/window.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace bgfx
{
class MAKINA_EXPORT context
{
public:
  static void initialize  (::bgfx::RendererType::Enum renderer_type, di::window* window);
  static void uninitialize();
};
}
}

#endif