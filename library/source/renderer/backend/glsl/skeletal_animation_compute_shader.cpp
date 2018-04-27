#include <makina/renderer/backend/glsl/skeletal_animation_compute_shader.hpp>

namespace mak
{
namespace glsl
{
std::string skeletal_animation_compute_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_KHR_vulkan_glsl : enable
#endif

struct _rig
{
  mat4 model ;
  mat4 offset;
};

layout(std430, set = 0, binding = 0)          buffer vertices
{
  vec4  vertex[];
};
layout(std430, set = 0, binding = 1)          buffer normals
{
  vec4  normal[];
};
layout(std430, set = 0, binding = 2) readonly buffer bone_ids
{
  uvec4 bone_id[];
};
layout(std430, set = 0, binding = 3) readonly buffer bone_weights
{
  vec4  bone_weight[];
};
layout(std430, set = 0, binding = 4) readonly buffer rig
{
  uvec4 rigs_metadata; // x size
  _rig  rigs[];
};

layout(local_size_variable)

void main()
{
  uint gid = gl_GlobalInvocationID.x;

  mat4 matrix = mat4(0.0);
  for (int i = 0; i < 4; ++i)
    if (bone_weights[i] > 0.0f)
      matrix += rigs[bone_ids[i]].model * rigs[bone_ids[i]].offset * bone_weights[i];
  output_vertex = (matrix * vec4(vertex, 1.0f)).xyz;
  output_normal = (matrix * vec4(normal, 0.0f)).xyz;
}
)";
}
}