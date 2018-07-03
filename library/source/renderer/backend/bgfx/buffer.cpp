#include <makina/renderer/backend/bgfx/buffer.hpp>

namespace mak
{
namespace bgfx
{
buffer::buffer           (const description& description)
{
  if      (description.type == description::type::vertex   )
    native_ = ::bgfx::createDynamicVertexBuffer(description.size, description.vertex_declaration);
  else if (description.type == description::type::index    )
    native_ = ::bgfx::createDynamicIndexBuffer(description.size);
  else if (description.type == description::type::draw_call)
    native_ = ::bgfx::createIndirectBuffer(description.size);
}
buffer::buffer           (buffer&& temp) noexcept : native_(temp.native_)
{
  std::get<::bgfx::DynamicVertexBufferHandle>(temp.native_).idx = ::bgfx::kInvalidHandle;
}
buffer::~buffer          ()
{
  if      (std::holds_alternative<::bgfx::DynamicVertexBufferHandle>(native_))
    ::bgfx::destroy(std::get     <::bgfx::DynamicVertexBufferHandle>(native_));
  else if (std::holds_alternative<::bgfx::DynamicIndexBufferHandle> (native_))
    ::bgfx::destroy(std::get     <::bgfx::DynamicIndexBufferHandle> (native_));
  else if (std::holds_alternative<::bgfx::IndirectBufferHandle>     (native_))
    ::bgfx::destroy(std::get     <::bgfx::IndirectBufferHandle>     (native_));
}
buffer& buffer::operator=(buffer&& temp) noexcept
{
  if (&temp != this)
  {
    native_ = temp.native_;
    std::get<::bgfx::DynamicVertexBufferHandle>(temp.native_).idx = ::bgfx::kInvalidHandle;
  }
  return *this;
}
}
}