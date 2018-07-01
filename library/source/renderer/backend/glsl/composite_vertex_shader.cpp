#include <makina/renderer/backend/glsl/composite_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string composite_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

layout(location = 0) in  vec2 vertex;
layout(location = 0) out vec2 texture_coordinate;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
};
#endif

void main() 
{
  texture_coordinate = (vertex + vec2(1.0f)) / 2.0f;
  gl_Position        = vec4(vertex.xy, 0.0f, 1.0f);
}
)";
}
}