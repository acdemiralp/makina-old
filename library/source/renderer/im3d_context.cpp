#include <makina/renderer/im3d_context.hpp>

namespace mak
{
Im3d::Context& im3d_context()
{
  return Im3d::GetContext();
}
}
