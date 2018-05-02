#include <makina/renderer/backend/glsl/immediate_triangles_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string immediate_triangles_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

layout(std430, set = 0, binding = 0) readonly buffer _projection
{
  mat4 projection;
};

layout(location = 0) in  vec4 attributes;
layout(location = 1) in  vec4 color     ;
layout(location = 0) out vs_output_type 
{
  vec4 color;
} vs_output;

void main()
{
  vs_output.color = color.abgr;
  gl_Position     = projection * vec4(attributes.xyz, 1.0f);
}
)";
}
}