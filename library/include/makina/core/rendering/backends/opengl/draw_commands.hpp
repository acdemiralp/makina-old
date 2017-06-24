#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_DRAW_COMMANDS_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_DRAW_COMMANDS_HPP_

#include <tuple>
#include <utility>
#include <vector>

#include <makina/core/rendering/backends/opengl/opengl.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
// 7.12.2 Shader memory access.
MAKINA_EXPORT void memory_barrier          (GLbitfield barrier_bits = GL_ALL_BARRIER_BITS);
MAKINA_EXPORT void memory_barrier_by_region(GLbitfield barrier_bits = GL_ALL_BARRIER_BITS);

// 9.3.1 Feedback loops.
MAKINA_EXPORT void texture_barrier();

// 10.3.6 Primitive restart.
MAKINA_EXPORT void set_primitive_restart_enabled(bool enabled, bool fixed_index = false);
MAKINA_EXPORT bool primitive_restart_enabled    (              bool fixed_index = false);

// 10.4 Drawing commands.
struct MAKINA_EXPORT draw_arrays_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_instance;
};
struct MAKINA_EXPORT draw_elements_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_vertex;
  GLuint base_instance;
};

MAKINA_EXPORT void draw_arrays                                      (GLenum mode, GLint offset, GLsizei count);
MAKINA_EXPORT void draw_arrays_instanced                            (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count = 1);
MAKINA_EXPORT void draw_arrays_instanced_base_intance               (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count = 1, GLuint base_instance = 0);
MAKINA_EXPORT void draw_arrays_indirect                             (GLenum mode, GLint offset = 0);

MAKINA_EXPORT void multi_draw_arrays                                (GLenum mode, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs);
MAKINA_EXPORT void multi_draw_arrays_indirect                       (GLenum mode, const GLint offset, GLsizei draw_count, GLsizei stride = sizeof(draw_arrays_indirect_command));

MAKINA_EXPORT void draw_elements                                    (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr);
MAKINA_EXPORT void draw_elements_instanced                          (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1);
MAKINA_EXPORT void draw_elements_instanced_base_instance            (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLuint base_instance = 0);
MAKINA_EXPORT void draw_range_elements                              (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices = nullptr);
MAKINA_EXPORT void draw_elements_indirect                           (GLenum mode, GLenum type, const GLint offset = 0);

MAKINA_EXPORT void multi_draw_elements                              (GLenum mode, GLenum type, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs);
MAKINA_EXPORT void multi_draw_elements_indirect                     (GLenum mode, GLenum type, const GLint offset, GLsizei draw_count, GLsizei stride = sizeof(draw_elements_indirect_command));

MAKINA_EXPORT void draw_elements_base_vertex                        (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLint base_vertex = 0);
MAKINA_EXPORT void draw_elements_instanced_base_vertex              (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLint base_vertex = 0);
MAKINA_EXPORT void draw_elements_instanced_base_vertex_base_instance(GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLint base_vertex = 0, GLuint base_instance = 0);
MAKINA_EXPORT void draw_range_elements_base_vertex                  (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices = nullptr, GLint base_vertex = 0);

MAKINA_EXPORT void multi_draw_elements_base_vertex                  (GLenum mode, GLenum type, const std::vector<std::tuple<GLint, GLsizei, GLint>>& offset_count_base_vertex_triplets);
}
}

#endif
