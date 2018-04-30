#include <makina/renderer/backend/glsl/skeletal_animation_compute_shader.hpp>

namespace mak
{
namespace glsl
{
std::string skeletal_animation_compute_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_compute_variable_group_size : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

struct _rig
{
  mat4 model ;
  mat4 offset;
};

layout(std430, set = 0, binding = 0)          buffer vertex
{
  vec4  vertices[];
};
layout(std430, set = 0, binding = 1)          buffer normal
{
  vec4  normals[];
};
layout(std430, set = 0, binding = 2) readonly buffer bone_id
{
  uvec4 bone_ids[];
};
layout(std430, set = 0, binding = 3) readonly buffer bone_weight
{
  vec4  bone_weights[];
};
layout(std430, set = 0, binding = 4) readonly buffer rig
{
  uvec4 rigs_metadata; // x size
  _rig  rigs[];
};

layout(local_size_variable) in;

void main()
{
  uint index  = gl_GlobalInvocationID.x;
  mat4 matrix = mat4(0.0);
  for (int i = 0; i < 4; ++i)
    if (bone_weights[index][i] > 0.0f)
      matrix += bone_weights[index][i] * rigs[bone_ids[index][i]].model * rigs[bone_ids[index][i]].offset;
  vertices[index] = matrix * vertices[index];
  normals [index] = matrix * normals [index];
}
)";
}
}