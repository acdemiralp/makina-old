#ifndef MAKINA_RENDERER_BACKEND_BGFX_BUFFER_HPP
#define MAKINA_RENDERER_BACKEND_BGFX_BUFFER_HPP

#include <memory>
#include <variant>

#include <bgfx/bgfx.h>
#include <fg/resource.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace bgfx
{
class MAKINA_EXPORT buffer
{
public:
  struct MAKINA_EXPORT description
  {
    enum class type
    {
      vertex,
      index ,
      draw_call
    };

    type               type              ;
    std::uint32_t      size              ;
    ::bgfx::VertexDecl vertex_declaration;
  };

  explicit buffer  (const description& description);
  buffer           (const buffer&  that) = delete;
  buffer           (      buffer&& temp) noexcept;
  virtual ~buffer  ()                    ;
  buffer& operator=(const buffer&  that) = delete;
  buffer& operator=(      buffer&& temp) noexcept;

  template<typename type>
  const type& native() const
  {
    return std::get<type>(native_);
  }

protected:
  std::variant<::bgfx::DynamicVertexBufferHandle, ::bgfx::DynamicIndexBufferHandle, ::bgfx::IndirectBufferHandle> native_;
};

using buffer_resource = fg::resource<buffer::description, buffer>;
}
}

template<>
inline std::unique_ptr<mak::bgfx::buffer> fg::realize(const mak::bgfx::buffer::description& description)
{
  return std::make_unique<mak::bgfx::buffer>(description);
}

#endif
