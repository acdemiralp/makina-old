#include <makina/renderer/backend/glsl/test_fragment_shader.hpp>

namespace mak
{
namespace glsl
{
std::string test_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#endif

layout(location = 0) in  vec3 color     ;
layout(location = 0) out vec4 frag_color;

void main() 
{
  frag_color = vec4(color.x + 0.5f, 0.5f, color.y + 0.5f, 1.0f);
}
)";
}
}