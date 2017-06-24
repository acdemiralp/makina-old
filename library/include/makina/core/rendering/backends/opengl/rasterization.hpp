#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_RASTERIZATION_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_RASTERIZATION_HPP_

#include <array>

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 14.0 Rasterization.
MAKINA_EXPORT void set_rasterizer_discard_enabled(bool enabled);
MAKINA_EXPORT bool rasterizer_discard_enabled    ();

// 14.3.1 Multisampling.
MAKINA_EXPORT void set_multisampling_enabled (bool enabled);
MAKINA_EXPORT bool multisampling_enabled     ();
MAKINA_EXPORT void set_sample_shading_enabled(bool enabled);
MAKINA_EXPORT bool sample_shading_enabled    ();

MAKINA_EXPORT void set_minimum_sample_shading(GLfloat value = 1.0);

MAKINA_EXPORT std::array<GLfloat, 2> multisample_sample_position(GLuint index);

// 14.4 Points.
MAKINA_EXPORT void set_point_size_enabled(bool enabled);
MAKINA_EXPORT bool point_size_enabled    ();

MAKINA_EXPORT void set_point_size               (GLfloat size   = 1.0);
MAKINA_EXPORT void set_point_fade_threshold_size(GLfloat size   = 1.0);
MAKINA_EXPORT void set_point_sprite_coord_origin(GLenum  origin = GL_UPPER_LEFT);

// 14.5 Line segments.
MAKINA_EXPORT void set_line_smoothing_enabled(bool enabled);
MAKINA_EXPORT bool line_smoothing_enabled    ();

MAKINA_EXPORT void set_line_width(GLfloat width);

// 14.6 Polygons.
MAKINA_EXPORT void set_polygon_smoothing_enabled(bool enabled);
MAKINA_EXPORT bool polygon_smoothing_enabled    ();

MAKINA_EXPORT void set_polygon_face_culling_enabled(bool enabled);
MAKINA_EXPORT bool polygon_face_culling_enabled    ();

MAKINA_EXPORT void set_front_face(GLenum mode = GL_CCW );
MAKINA_EXPORT void set_cull_face (GLenum mode = GL_BACK);

// 14.6.4 Polygon rasterization and depth offset.
MAKINA_EXPORT void set_polygon_mode  (GLenum mode = GL_FILL);
MAKINA_EXPORT void set_polygon_offset(GLfloat factor, GLfloat units);

MAKINA_EXPORT void set_polygon_point_offset_enabled(bool enabled);
MAKINA_EXPORT bool polygon_point_offset_enabled    ();
MAKINA_EXPORT void set_polygon_line_offset_enabled (bool enabled);
MAKINA_EXPORT bool polygon_line_offset_enabled     ();
MAKINA_EXPORT void set_polygon_fill_offset_enabled (bool enabled);
MAKINA_EXPORT bool polygon_fill_offset_enabled     ();
}
}

#endif
