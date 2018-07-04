#include <makina/renderer/backend/bgfx/context.hpp>

#include <bgfx/platform.h>

namespace mak
{
namespace bgfx
{
void context::initialize  (const ::bgfx::RendererType::Enum renderer_type, di::window* window)
{
  if (!window)
    throw std::runtime_error("Failed to initialize bgfx: No window specified for setting platform data.");

  SDL_SysWMinfo platform_info;
  SDL_VERSION(&platform_info.version);
  if (!SDL_GetWindowWMInfo(window->native(), &platform_info))
    throw std::runtime_error("Failed to initialize bgfx: Unable to retrieve window platform data.");

  ::bgfx::PlatformData platform_data;  
  platform_data.context      = nullptr;
  platform_data.backBuffer   = nullptr;
  platform_data.backBufferDS = nullptr;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  platform_data.ndt = platform_info.info.x11.display;
  platform_data.nwh = (void*) (uintptr_t) platform_info.info.x11.window;
#elif BX_PLATFORM_OSX
  platform_data.ndt = nullptr;
  platform_data.nwh = platform_info.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
  platform_data.ndt = nullptr;
  platform_data.nwh = platform_info.info.win.window;
#endif
  ::bgfx::setPlatformData(platform_data);

  if (!::bgfx::init(renderer_type))
    throw std::runtime_error("Failed to initialize bgfx.");

  window->on_resize.connect([ ] (const std::array<std::size_t, 2>& size)
  {
    ::bgfx::reset(size[0], size[1], BGFX_RESET_VSYNC);
  });
  ::bgfx::reset(window->size()[0], window->size()[1], BGFX_RESET_VSYNC);
  
  ::bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 10u, 1.0f);
  ::bgfx::setViewRect (0, 0u, 0u, window->size()[0], window->size()[1]);

#ifndef NDEBUG
  ::bgfx::setDebug(BGFX_DEBUG_STATS | BGFX_DEBUG_TEXT);
#endif

  ::bgfx::frame() ;
  ::bgfx::touch(0);
}
void context::uninitialize()
{
  ::bgfx::shutdown();
}
}
}