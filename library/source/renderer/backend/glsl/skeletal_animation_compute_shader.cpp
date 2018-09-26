#include <makina/renderer/backend/glsl/skeletal_animation_compute_shader.hpp>

namespace mak
{
namespace glsl
{
std::string skeletal_animation_compute_shader = R"(
#version 450

#ifdef VULKAN

#else

#ifdef GL_ARB_compute_variable_group_size
#extension GL_ARB_compute_variable_group_size : enable
#endif

#endif

struct _rig
{
  mat4 model ;
  mat4 offset;
};

layout(std430, binding = 0)          buffer vertex
{
  vec4  vertices[];
};
layout(std430, binding = 1)          buffer normal
{
  vec4  normals[];
};
layout(std430, binding = 2) readonly buffer bone_id
{
  uvec4 bone_ids[];
};
layout(std430, binding = 3) readonly buffer bone_weight
{
  vec4  bone_weights[];
};
layout(std430, binding = 4) readonly buffer rig
{
  uvec4 rigs_metadata; // x size
  _rig  rigs[];
};

#ifdef GL_ARB_compute_variable_group_size
layout (local_size_variable) in;
#else
layout (local_size_x = 1, local_size_y = 1) in;
#endif

void main()
{
  uint index  = gl_GlobalInvocationID.x;
  mat4 matrix = mat4(0.0);
  for (int i = 0; i < 4; ++i)
    if (bone_weights[index][i] > 0.0f)
      matrix += bone_weights[index][i] * rigs[bone_ids[index][i]].model * rigs[bone_ids[index][i]].offset;
  if (matrix == mat4(0.0)) return;
  vertices[index] = matrix * vertices[index];
  normals [index] = matrix * normals [index];
}
)";
}
}