#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_PER_FRAGMENT_OPS_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_PER_FRAGMENT_OPS_HPP_

#include <array>
#include <vector>

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 17.3.2 Scissor test.
MAKINA_EXPORT void set_scissor_test_enabled(bool enabled);
MAKINA_EXPORT bool scissor_test_enabled    ();
MAKINA_EXPORT void set_scissor             (const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);

MAKINA_EXPORT void set_indexed_scissor_test_enabled(GLuint index, bool enabled);
MAKINA_EXPORT bool indexed_scissor_test_enabled    (GLuint index);
MAKINA_EXPORT void set_indexed_scissor             (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);

// 17.3.3 Multisample fragment operations.
MAKINA_EXPORT void set_sample_alpha_to_coverage_enabled(bool enabled);
MAKINA_EXPORT bool sample_alpha_to_coverage_enabled    ();
MAKINA_EXPORT void set_sample_alpha_to_one_enabled     (bool enabled);
MAKINA_EXPORT bool sample_alpha_to_one_enabled         ();
MAKINA_EXPORT void set_sample_coverage_enabled         (bool enabled);
MAKINA_EXPORT bool sample_coverage_enabled             ();
MAKINA_EXPORT void set_sample_mask_enabled             (bool enabled);
MAKINA_EXPORT bool sample_mask_enabled                 ();
MAKINA_EXPORT void set_sample_coverage                 (GLfloat value, bool invert = false);
MAKINA_EXPORT void set_sample_mask                     (GLuint index, GLbitfield mask);

// 17.3.5 Stencil test.
MAKINA_EXPORT void set_stencil_test_enabled(bool enabled);
MAKINA_EXPORT bool stencil_test_enabled    ();
MAKINA_EXPORT void set_stencil_function    (GLenum function, GLint reference_value, GLuint mask         , GLenum face = GL_FRONT_AND_BACK);
MAKINA_EXPORT void set_stencil_operation   (GLenum stencil_fail, GLenum depth_fail, GLenum depth_success, GLenum face = GL_FRONT_AND_BACK);

// 17.3.6 Depth buffer test.
MAKINA_EXPORT void set_depth_test_enabled(bool enabled);
MAKINA_EXPORT bool depth_test_enabled    ();
MAKINA_EXPORT void set_depth_function    (GLenum function = GL_LESS);

// 17.3.8 Blending.
MAKINA_EXPORT void set_blending_enabled(bool enabled);
MAKINA_EXPORT bool blending_enabled    ();
MAKINA_EXPORT void set_blend_equation  (GLenum mode       = GL_FUNC_ADD);
MAKINA_EXPORT void set_blend_equation  (GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD);
MAKINA_EXPORT void set_blend_function  (GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO    );
MAKINA_EXPORT void set_blend_function  (GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE     , GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO);
MAKINA_EXPORT void set_blend_color     (const std::array<GLfloat, 4>& color);

MAKINA_EXPORT void set_indexed_blending_enabled(GLuint index, bool enabled);
MAKINA_EXPORT bool indexed_blending_enabled    (GLuint index);
MAKINA_EXPORT void set_indexed_blend_equation  (GLuint index, GLenum mode       = GL_FUNC_ADD);
MAKINA_EXPORT void set_indexed_blend_equation  (GLuint index, GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD);
MAKINA_EXPORT void set_indexed_blend_function  (GLuint index, GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO);
MAKINA_EXPORT void set_indexed_blend_function  (GLuint index, GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE, GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO);

// 17.3.10 Dithering.
MAKINA_EXPORT void set_dithering_enabled(bool enabled);
MAKINA_EXPORT bool dithering_enabled    ();

// 17.3.11 Logical operation.
MAKINA_EXPORT void set_logic_operation_enabled(bool enabled);
MAKINA_EXPORT bool logic_operation_enabled    ();
MAKINA_EXPORT void set_logic_operation        (GLenum operation = GL_COPY);
}
}

#endif