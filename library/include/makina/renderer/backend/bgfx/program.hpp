#ifndef MAKINA_RENDERER_BACKEND_BGFX_PROGRAM_HPP_
#define MAKINA_RENDERER_BACKEND_BGFX_PROGRAM_HPP_

#include <memory>
#include <string>

#include <bgfx/bgfx.h>
#include <fg/resource.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace bgfx
{
class MAKINA_EXPORT program
{
public:
  struct MAKINA_EXPORT description
  {
    std::string vertex_shader_source  ;
    std::string fragment_shader_source;
  };

  explicit program(const description& description);
  program           (const program&  that) = delete;
  program           (      program&& temp) noexcept;
  virtual ~program();
  program& operator=(const program&  that) = delete;
  program& operator=(      program&& temp) noexcept;

  const ::bgfx::ProgramHandle& native() const;

protected:
  ::bgfx::ProgramHandle native_;
};

using program_resource = fg::resource<program::description, program>;
}
}

template<>
inline std::unique_ptr<mak::bgfx::program> fg::realize(const mak::bgfx::program::description& description)
{
  return std::make_unique<mak::bgfx::program>(description);
}

#endif
