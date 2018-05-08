#include <makina/renderer/backend/glsl/default_vertex_shader.hpp>

namespace mak
{
namespace glsl
{
std::string default_vertex_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

struct _transform
{
  mat4 model     ;
};
struct _camera
{
  mat4 view      ;
  mat4 projection;
};

layout(std430, set = 0, binding = 0) readonly buffer transform
{
  uvec4      transforms_metadata; // x size
  _transform transforms[];
};
layout(std430, set = 0, binding = 2) readonly buffer camera
{
  uvec4      cameras_metadata; // x size, y index
  _camera    cameras   [];
};

layout(location = 0) in vec4  vertex;
layout(location = 1) in vec4  normal;
layout(location = 2) in vec3  texture_coordinate;
layout(location = 3) in uvec2 instance_attribute; // x transform id, y material id

layout(location = 0) out vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} vs_output;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
};
#endif

void main()
{
  mat4 model_view   = cameras[cameras_metadata.y].view * transforms[instance_attribute.x].model;
  vec4 trans_vertex = model_view * vertex;
  
  vs_output.vertex             = trans_vertex.xyz;
  vs_output.normal             = normalize((model_view * normal).xyz);
  vs_output.texture_coordinate = texture_coordinate.xy;
  vs_output.material_index     = instance_attribute.y;

  gl_Position = cameras[cameras_metadata.y].projection * trans_vertex;
}
)";
}
}