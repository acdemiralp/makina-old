#ifndef MAKINA_RENDERER_BACKEND_OPENGL_GRAPHICS_PIPELINE_HPP_
#define MAKINA_RENDERER_BACKEND_OPENGL_GRAPHICS_PIPELINE_HPP_

#include <optional>

#include <gl/program.hpp>
#include <gl/shader.hpp>

#include <makina/export.hpp>

namespace mak
{
class MAKINA_EXPORT opengl_graphics_pipeline : public gl::program
{
public:
  explicit opengl_graphics_pipeline(
    const std::string& vertex_stage                  = std::string(),
    const std::string& tessellation_control_stage    = std::string(),
    const std::string& tessellation_evaluation_stage = std::string(),
    const std::string& geometry_stage                = std::string(),
    const std::string& fragment_stage                = std::string());
  opengl_graphics_pipeline           (const opengl_graphics_pipeline&  that) = delete ;
  opengl_graphics_pipeline           (      opengl_graphics_pipeline&& temp) = default;
  virtual ~opengl_graphics_pipeline  ()                                      = default;
  opengl_graphics_pipeline& operator=(const opengl_graphics_pipeline&  that) = delete ;
  opengl_graphics_pipeline& operator=(      opengl_graphics_pipeline&& temp) = default;

protected:
  std::optional<gl::shader> vertex_stage_                 ;
  std::optional<gl::shader> tessellation_control_stage_   ;
  std::optional<gl::shader> tessellation_evaluation_stage_;
  std::optional<gl::shader> geometry_stage_               ;
  std::optional<gl::shader> fragment_stage_               ;
};
}

#endif