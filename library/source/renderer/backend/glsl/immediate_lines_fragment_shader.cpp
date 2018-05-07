#include <makina/renderer/backend/glsl/immediate_lines_fragment_shader.hpp>

namespace mak
{
namespace glsl
{
std::string immediate_lines_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

const float antialiasing = 2.0f;

layout(location = 0) in vs_output_type 
{
  noperspective float edge_distance;
  noperspective float size         ;
  vec4                color        ;
} fs_input;

layout(location = 0) out vec4 color;

void main()
{
  color    = fs_input.color;
  color.a *= smoothstep(1.0f, 1.0f - (antialiasing / fs_input.size), abs(fs_input.edge_distance) / fs_input.size);
}
)";
}
}