#include <makina/renderer/backend/glsl/composite_fragment_shader.hpp>

namespace mak
{
namespace glsl
{
std::string composite_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;
#endif

const float alpha_threshold = 0.01f;

layout(binding = 0) uniform sampler2D source_color;
layout(binding = 1) uniform sampler2D source_depth;

layout(location = 0) in  vec2 texture_coordinate;
layout(location = 0) out vec4 output_color;

#ifdef VULKAN
out float gl_FragDepth;
#endif

void main()
{
  vec4  color = texture(source_color, texture_coordinate).xyzw;
  float depth = texture(source_depth, texture_coordinate).x;
  if (color.w < alpha_threshold) discard;
  output_color = color;	
  gl_FragDepth = depth;
}
)";
}
}