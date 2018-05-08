#include <makina/renderer/backend/glsl/immediate_lines_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string immediate_lines_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

const float antialiasing = 2.0f;

struct _camera
{
  mat4 view      ;
  mat4 projection;
};

layout(std430, set = 0, binding = 0) readonly buffer camera
{
  uvec4     cameras_metadata  ; // x size, y index
  _camera   cameras[]         ;
};

layout(location = 0) in  vec4 attributes;
layout(location = 1) in  vec4 color     ;
layout(location = 0) out vs_output_type 
{
  noperspective float edge_distance;
  noperspective float size         ;
  vec4                color        ;
} vs_output;

void main()
{
  vs_output.size     = max(attributes.w, antialiasing);
  vs_output.color    = color.abgr;
  vs_output.color.a *= smoothstep(0.0f, 1.0f, attributes.w / antialiasing);
  gl_Position        = cameras[cameras_metadata.y].projection * vec4(attributes.xyz, 1.0f);
}
)";
}
}