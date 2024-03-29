#ifndef MAKINA_RENDERER_BACKEND_OPENGL_PROGRAM_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_PROGRAM_HPP

#include <optional>

#include <fg/resource.hpp>
#include <gl/program.hpp>
#include <gl/shader.hpp>

#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
// A gl::program with in-built shaders.
class MAKINA_EXPORT program : public gl::program
{
public:
  struct MAKINA_EXPORT compute_description
  {
    std::string compute_stage;
  };
  struct MAKINA_EXPORT graphics_description
  {
    // Order is intentionally different based on most common usage for convenience.
    std::string vertex_stage                 ;
    std::string fragment_stage               ;
    std::string geometry_stage               ;
    std::string tessellation_control_stage   ;
    std::string tessellation_evaluation_stage;
  };

  explicit program  (const compute_description&  description);
  explicit program  (const graphics_description& description);
  program           (const program&  that) = delete ;
  program           (      program&& temp) = default;
  virtual ~program  ()                     = default;
  program& operator=(const program&  that) = delete ;
  program& operator=(      program&& temp) = default;

protected:
  std::optional<gl::shader> compute_stage_                ;
  std::optional<gl::shader> vertex_stage_                 ;
  std::optional<gl::shader> tessellation_control_stage_   ;
  std::optional<gl::shader> tessellation_evaluation_stage_;
  std::optional<gl::shader> geometry_stage_               ;
  std::optional<gl::shader> fragment_stage_               ;
};
using compute_program_resource  = fg::resource<program::compute_description , program>;
using graphics_program_resource = fg::resource<program::graphics_description, program>;
}
}

template<>
inline std::unique_ptr<mak::opengl::program> fg::realize(const mak::opengl::program::compute_description& description)
{
  return std::make_unique<mak::opengl::program>(description);
}
template<>
inline std::unique_ptr<mak::opengl::program> fg::realize(const mak::opengl::program::graphics_description& description)
{
  return std::make_unique<mak::opengl::program>(description);
}

#endif