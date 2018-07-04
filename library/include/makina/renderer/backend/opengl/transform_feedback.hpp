#ifndef MAKINA_RENDERER_BACKEND_OPENGL_TRANSFORM_FEEDBACK_HPP
#define MAKINA_RENDERER_BACKEND_OPENGL_TRANSFORM_FEEDBACK_HPP

#include <fg/resource.hpp>
#include <gl/transform_feedback.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT transform_feedback_description
{
  std::vector<buffer_resource*> buffers;
};
using transform_feedback_resource = fg::resource<transform_feedback_description, gl::transform_feedback>;
}
}

template<>
inline std::unique_ptr<gl::transform_feedback> fg::realize(const mak::opengl::transform_feedback_description& description)
{
  auto   transform_feedback = std::make_unique<gl::transform_feedback>();
  for(auto i = 0; i < description.buffers.size(); ++i)
    transform_feedback->set_buffer_base(i, *description.buffers[i]->actual());
  return transform_feedback;
}

#endif