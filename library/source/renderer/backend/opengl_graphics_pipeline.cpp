#include <makina/renderer/backend/opengl_graphics_pipeline.hpp>

namespace mak
{
opengl_graphics_pipeline::opengl_graphics_pipeline(
  const std::string& vertex_stage                 ,
  const std::string& tessellation_control_stage   ,
  const std::string& tessellation_evaluation_stage,
  const std::string& geometry_stage               ,
  const std::string& fragment_stage               )
{
  if (vertex_stage.size())
  {
    vertex_stage_. emplace   (GL_VERTEX_SHADER);
    vertex_stage_->set_source(vertex_stage);
    vertex_stage_->compile   ();
    attach_shader(vertex_stage_.value());
  }
  
  if (tessellation_control_stage.size())
  {
    tessellation_control_stage_. emplace   (GL_TESS_CONTROL_SHADER);
    tessellation_control_stage_->set_source(tessellation_control_stage);
    tessellation_control_stage_->compile   ();
    attach_shader(tessellation_control_stage_.value());
  }

  if (tessellation_evaluation_stage.size())
  {
    tessellation_evaluation_stage_. emplace   (GL_TESS_EVALUATION_SHADER);
    tessellation_evaluation_stage_->set_source(tessellation_evaluation_stage);
    tessellation_evaluation_stage_->compile   ();
    attach_shader(tessellation_evaluation_stage_.value());
  }
  
  if (geometry_stage.size())
  {
    geometry_stage_. emplace   (GL_GEOMETRY_SHADER);
    geometry_stage_->set_source(geometry_stage);
    geometry_stage_->compile   ();
    attach_shader(geometry_stage_.value());
  }
  
  if (fragment_stage.size())
  {
    fragment_stage_. emplace   (GL_FRAGMENT_SHADER);
    fragment_stage_->set_source(fragment_stage);
    fragment_stage_->compile   ();
    attach_shader(fragment_stage_.value());
  }

  link();
}
}
