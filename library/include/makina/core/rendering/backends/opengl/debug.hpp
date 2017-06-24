#ifndef MAKINA_CORE_RENDERING_BACKENDS_OPENGL_DEBUG_HPP_
#define MAKINA_CORE_RENDERING_BACKENDS_OPENGL_DEBUG_HPP_

#include <functional>
#include <string>
#include <vector>

#include <makina/core/rendering/backends/opengl/opengl.hpp>
#include <makina/core/rendering/backends/opengl/sync.hpp>

#include <export.hpp>

namespace mak
{
namespace gl
{
struct debug_log
{
  GLenum      source   ;
  GLenum      type     ;
  GLuint      id       ;
  GLenum      severity ;
  std::string message  ;
  const void* user_data;
};

// 20.0 Debug output.
MAKINA_EXPORT void set_debug_output_enabled(bool enabled);
MAKINA_EXPORT bool debug_output_enabled    ();

// 20.2 Debug message callback.
MAKINA_EXPORT void set_debug_log_callback(const std::function<void(const debug_log&)>& callback, void* user_data = nullptr);

// 20.4 Controlling debug messages.
MAKINA_EXPORT void set_debug_log_filters(GLenum source, GLenum type, const std::vector<GLuint>& ids, GLenum severity, bool enabled);

// 20.5 Externally generated messages.
MAKINA_EXPORT void insert_debug_log(const debug_log& log);

// 20.6 Debug groups.
MAKINA_EXPORT void push_debug_group(GLenum source, GLuint id, const std::string& message);
MAKINA_EXPORT void pop_debug_group ();

// 20.7 Debug labels.
template<typename type>
void set_object_label     (const type& object     , const std::string& label)
{
  glObjectLabel(type::native_type, object.id(), label.size(), label.data());
}
MAKINA_EXPORT void set_sync_object_label(const sync& sync_object, const std::string& label);

// 20.8 Synchronous Debug output.
MAKINA_EXPORT void set_syncronous_debug_output_enabled(bool enabled);
MAKINA_EXPORT bool synchronous_debug_output_enabled   ();

// 20.9 Debug output queries.
MAKINA_EXPORT std::vector<debug_log> debug_logs(GLuint count);

template<typename type>
std::string object_label     (const type& object)
{
  GLint       size ; glGetIntegerv(GL_MAX_LABEL_LENGTH, &size);
  std::string label; label.resize(size);
  glGetObjectLabel(type::native_type, object.id(), size, nullptr, &label[0]);
  return label;
}
MAKINA_EXPORT std::string sync_object_label(const sync& sync_object);
}
}

#endif