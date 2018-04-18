#include <makina/renderer/backend/glsl/skeletal_animation_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string skeletal_animation_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

struct _transform
{
  mat4 model ;
};
struct _rig
{
  mat4 offset;
};

layout(std430, set = 0, binding = 0) readonly buffer transform
{
  uvec4      transforms_metadata; // x size
  _transform transforms[];
};
layout(std430, set = 0, binding = 1) readonly buffer rig
{
  uvec4      rigs_metadata;       // x size
  _rigs      rigs[];
};

layout(location = 0) in vec3  vertex      ;
layout(location = 1) in vec3  normal      ;
layout(location = 2) in uvec4 bone_ids    ;
layout(location = 3) in vec4  bone_weights;

layout(location = 0, xfb_buffer = 0) out vec3 output_vertex;
layout(location = 1, xfb_buffer = 1) out vec3 output_normal;

void main()
{
  mat4 matrix = mat4(0.0);
  for (int i = 0; i < 4; ++i)
    if (bone_weights[i] > 0.0f)
      matrix += transforms[bone_ids[i]] * rigs[bone_ids[i]] * bone_weights[i];
  output_vertex = matrix * vec4(vertex, 1.0f);
  output_normal = matrix * vec4(normal, 0.0f);
}
)";
}
}