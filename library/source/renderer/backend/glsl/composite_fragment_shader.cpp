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
#extension GL_KHR_vulkan_glsl : enable
layout (bindless_sampler) uniform;
#endif

float linearize(float z, float near, float far)
{
  return (z * far - 2 * near + z * near) / (z * far - z * near);
}

layout(set = 0, binding = 0) uniform sampler2D source_color;
layout(set = 0, binding = 1) uniform sampler2D source_depth;

layout(location = 0) in  vec2 texture_coordinate;
layout(location = 0) out vec4 output_color;

#ifdef VULKAN
out float gl_FragDepth;
#endif

**
struct parameters
{
  bool  is_orthographic;
  float near           ;
  float far            ;
  float alpha_threshold;
}
**

void main()
{
  vec4  color = texture(source_color, texture_coordinate).xyzw;
  float depth = texture(source_depth, texture_coordinate).x;

  if (color.w < **alpha_threshold**) 
    discard;

  output_color = color;	
  gl_FragDepth = linearize(depth, **near**, **far**);
}
)";
}
}