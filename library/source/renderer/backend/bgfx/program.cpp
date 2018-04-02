#include <makina/renderer/backend/bgfx/program.hpp>

namespace mak
{
namespace bgfx
{
program::program(const description& description)
{
  const auto vertex_shader   = ::bgfx::copy(description.vertex_shader_source  .data(), description.vertex_shader_source  .size());
  const auto fragment_shader = ::bgfx::copy(description.fragment_shader_source.data(), description.fragment_shader_source.size());
  const auto vertex_handle   = createShader(vertex_shader  );
  const auto fragment_handle = createShader(fragment_shader);
  native_ = createProgram(vertex_handle, fragment_handle, true);
}
program::program(program&& temp) noexcept : native_(std::move(temp.native_))
{
  temp.native_.idx = ::bgfx::kInvalidHandle;
}
program::~program()
{
  if(native_.idx != ::bgfx::kInvalidHandle)
    destroy(native_);
}
program& program::operator=(program&& temp) noexcept
{
  if(&temp != this)
  {
    native_          = std::move(temp.native_);
    temp.native_.idx = ::bgfx::kInvalidHandle ;
  }
  return *this;
}

const ::bgfx::ProgramHandle& program::native() const
{
  return native_;
}
}
}