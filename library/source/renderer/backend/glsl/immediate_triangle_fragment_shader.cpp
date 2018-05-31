#include <makina/renderer/backend/glsl/immediate_triangles_fragment_shader.hpp>

namespace mak
{
namespace glsl
{
std::string immediate_triangles_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

layout(location = 0) in vs_output_type 
{
  vec4 color;
} fs_input;

layout(location = 0) out vec4 color;

void main()
{
  color = fs_input.color;
}
)";
}
}