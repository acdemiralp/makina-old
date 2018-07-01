#include <makina/renderer/backend/glsl/test_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string test_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

layout(location = 0) in  vec3 vertex;
layout(location = 0) out vec3 color ;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
};
#endif

void main() 
{
  color       = vertex           ;
  gl_Position = vec4(vertex, 1.0);
}
)";
}
}