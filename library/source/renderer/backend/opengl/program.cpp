#include <makina/renderer/backend/opengl/program.hpp>

#include <makina/core/logger.hpp>

namespace mak
{
namespace opengl
{
program::program(const description& description)
{
  if (description.vertex_stage.size() > 0)
  {
    vertex_stage_. emplace   (GL_VERTEX_SHADER);
    vertex_stage_->set_source(description.vertex_stage);
    vertex_stage_->compile   ();

    if (vertex_stage_->info_log_length() > 0) 
      logger->error(vertex_stage_->info_log());

    attach_shader(vertex_stage_.value());
  }
  
  if (description.tessellation_control_stage.size() > 0)
  {
    tessellation_control_stage_. emplace   (GL_TESS_CONTROL_SHADER);
    tessellation_control_stage_->set_source(description.tessellation_control_stage);
    tessellation_control_stage_->compile   ();

    if (tessellation_control_stage_->info_log_length() > 0)
      logger->error(tessellation_control_stage_->info_log());

    attach_shader(tessellation_control_stage_.value());
  }

  if (description.tessellation_evaluation_stage.size() > 0)
  {
    tessellation_evaluation_stage_. emplace   (GL_TESS_EVALUATION_SHADER);
    tessellation_evaluation_stage_->set_source(description.tessellation_evaluation_stage);
    tessellation_evaluation_stage_->compile   ();

    if (tessellation_evaluation_stage_->info_log_length() > 0)
      logger->error(tessellation_evaluation_stage_->info_log());

    attach_shader(tessellation_evaluation_stage_.value());
  }
  
  if (description.geometry_stage.size() > 0)
  {
    geometry_stage_. emplace   (GL_GEOMETRY_SHADER);
    geometry_stage_->set_source(description.geometry_stage);
    geometry_stage_->compile   ();

    if (geometry_stage_->info_log_length() > 0)
      logger->error(geometry_stage_->info_log());

    attach_shader(geometry_stage_.value());
  }
  
  if (description.fragment_stage.size() > 0)
  {
    fragment_stage_. emplace   (GL_FRAGMENT_SHADER);
    fragment_stage_->set_source(description.fragment_stage);
    fragment_stage_->compile   ();

    if (fragment_stage_->info_log_length() > 0)
      logger->error(fragment_stage_->info_log());

    attach_shader(fragment_stage_.value());
  }

  link();

  if(info_log_length() > 0)
    logger->error(info_log());
}
}
}
