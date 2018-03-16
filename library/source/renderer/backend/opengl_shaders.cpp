#include <makina/renderer/backend/opengl_shaders.hpp>

namespace mak
{
std::string phong_vertex_shader   = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3  vertex;
layout(location = 1) in vec3  normal;
layout(location = 2) in vec2  texture_coordinate;
layout(location = 3) in uvec2 instance_attribute; // x transform id, y material id

struct _transform
{
  mat4 model     ;
}
struct _camera
{
  mat4 view      ;
  mat4 projection;
}

layout(std430, binding = 0) readonly buffer transform
{
  uint       count;
  _transform transforms[];
}
layout(std430, binding = 1) readonly buffer camera
{
  uint       count;
  _camera    cameras   [];
}

uniform uint camera_index = 0;

out vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} output;

void main()
{
  mat4 model_view   = cameras[camera_index].view * transforms[instance_attribute.x].model;
  vec4 trans_vertex = model_view * vec4(vertex, 1.0f);
  
  output.vertex             = trans_vertex.xyz;
  output.normal             = normalize((model_view * vec4(normal, 0.0f)).xyz);
  output.texture_coordinate = texture_coordinate;
  output.material_index     = instance_attribute

  gl_Position = cameras[camera_index].projection * trans_vertex;
}
)";
std::string phong_fragment_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable

layout (bindless_sampler) uniform;

struct _camera
{
  mat4      view            ;
  mat4      projection      ;
}
struct _material
{
  bvec3     use_texture     ; // ambient-diffuse-specular
  vec3      ambient         ;
  vec3      diffuse         ;
  vec3      specular        ;
  float     shininess       ;
  sampler2D ambient_texture ;
  sampler2D diffuse_texture ;
  sampler2D specular_texture;
}
struct _light
{
  uint      type            ;
  vec3      color           ;
  float     intensity       ;
  float     range           ; // spot-point
  float     spot_angle      ; // spot
  vec3      position        ; // spot-point
  vec3      direction       ; // spot-directional
}

layout(std430, binding = 1) readonly buffer camera
{
  uint      count;
  _camera   cameras  [];
}
layout(std430, binding = 2) readonly buffer material
{
  uint      count;
  _material materials[];
}
layout(std430, binding = 3) readonly buffer light
{
  uint      count;
  _light    lights   [];
}

uniform uint camera_index = 0;

in vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} input;

void main()
{
  
}
)";
}