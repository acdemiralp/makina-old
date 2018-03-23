#include <makina/renderer/backend/glsl/shader_sources.hpp>

namespace mak
{
std::string default_vertex_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable

struct _transform
{
  mat4 model     ;
};
struct _camera
{
  mat4 view      ;
  mat4 projection;
};

layout(location = 0) in vec3  vertex;
layout(location = 1) in vec3  normal;
layout(location = 2) in vec3  texture_coordinate;
layout(location = 3) in uvec2 instance_attribute; // x transform id, y material id

layout(std430, binding = 0) readonly buffer transform
{
  uint       transforms_size;
  _transform transforms[];
};
layout(std430, binding = 2) readonly buffer camera
{
  uint       cameras_size;
  _camera    cameras   [];
};
uniform uint camera_index = 0u;

out vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} vs_output;

void main()
{
  mat4 model_view   = cameras[camera_index].view * transforms[instance_attribute.x].model;
  vec4 trans_vertex = model_view * vec4(vertex, 1.0);
  
  vs_output.vertex             = trans_vertex.xyz;
  vs_output.normal             = normalize((model_view * vec4(normal, 0.0f)).xyz);
  vs_output.texture_coordinate = texture_coordinate.xy;
  vs_output.material_index     = instance_attribute.y;

  gl_Position = cameras[camera_index].projection * trans_vertex;
}
)";
std::string phong_fragment_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;

const uint light_type_ambient     = 0;
const uint light_type_directional = 1;
const uint light_type_point       = 2;
const uint light_type_spot        = 3;

struct _material
{
  uvec4     use_texture     ; // ambient - diffuse - specular - unused
  vec4      ambient         ; // w is unused.
  vec4      diffuse         ; // w is unused.
  vec4      specular        ; // w is shininess.
  sampler2D ambient_texture ;
  sampler2D diffuse_texture ;
  sampler2D specular_texture;
  sampler2D unused_texture  ;
};
struct _camera
{
  mat4      view            ;
  mat4      projection      ;
};
struct _light
{
  uvec4     type            ; // y, z, w are unused. 
  vec4      color           ; // w is unused.
  vec4      properties      ; // intensity - range - inner spot - outer spot
  vec4      position        ; // w is unused.
  vec4      direction       ; // w is unused.
};

layout(std430, binding = 1) readonly buffer material
{
  uint      materials_size  ;
  _material materials[]     ;
};
layout(std430, binding = 2) readonly buffer camera
{
  uint      cameras_size    ;
  _camera   cameras[]       ;
};
layout(std430, binding = 3) readonly buffer light
{
  uint      lights_size     ;
  _light    lights[]        ;
};
uniform uint  camera_index          = 0;
uniform float attenuation_constant  = 1.0;
uniform float attenuation_linear    = 0.01;
uniform float attenuation_quadratic = 0.0001;

in vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} fs_input;

layout(location = 0) out vec4 output_color;

void main()
{
  vec3  ka = materials[fs_input.material_index].use_texture[0] == 1
    ? texture(materials[fs_input.material_index].ambient_texture , fs_input.texture_coordinate).rgb 
    : materials[fs_input.material_index].ambient.rgb ;             
  vec3  kd = materials[fs_input.material_index].use_texture[1] == 1
    ? texture(materials[fs_input.material_index].diffuse_texture , fs_input.texture_coordinate).rgb 
    : materials[fs_input.material_index].diffuse.rgb ;             
  vec3  ks = materials[fs_input.material_index].use_texture[2] == 1
    ? texture(materials[fs_input.material_index].specular_texture, fs_input.texture_coordinate).rgb 
    : materials[fs_input.material_index].specular.rgb;
  float a  = max(materials[fs_input.material_index].specular.w, 0.0001f);
  vec3  n  = normalize( fs_input.normal);
  vec3  v  = normalize(-fs_input.vertex);

  vec3 color = vec3(0.0f);
  for(uint i = 0; i < lights_size; ++i)
  {
    vec3 il   = lights[i].properties.x * lights[i].color.rgb;
    uint type = lights[i].type.x;

    if (type == light_type_ambient)
    {
      color += clamp(vec3(ka * il), 0.0f, 1.0f);
    }
    if (type == light_type_directional)
    {
      vec3  l           = -normalize(cameras[camera_index].view * vec4(lights[i].direction.xyz, 0.0)).xyz;
      vec3  r           =  reflect  (-l, n);
      vec3  diffuse     = kd * il * max(dot(n, l), 0.0f);
      vec3  specular    = ks * il * pow(max(dot(v, r), 0.0f), a);
      color += clamp(diffuse + specular, 0.0f, 1.0f);
    }
    if (type == light_type_point || type == light_type_spot)
    {
      vec3  p           = (cameras[camera_index].view * vec4(lights[i].position.xyz, 1.0f)).xyz;
      vec3  l           = normalize(p - fs_input.vertex);
      vec3  r           = reflect  (-l, n);
      float distance    = length   (p - fs_input.vertex);
      float attenuation = 1.0 / (attenuation_constant + attenuation_linear * distance + attenuation_quadratic * distance * distance);
      vec3  diffuse     = attenuation * kd * il * max(dot(n, l), 0.0f);
      vec3  specular    = attenuation * ks * il * pow(max(dot(v, r), 0.0f), a);
  
      // Soft-edged spotlights are a special case of point lights.
      if(type == light_type_spot)
      {
        float cutoff = clamp((dot(l, -normalize(cameras[camera_index].view * vec4(lights[i].direction.xyz, 0.0f)).xyz) - lights[i].properties.w) / (lights[i].properties.z - lights[i].properties.w), 0.0f, 1.0f);
        diffuse  *= cutoff;
        specular *= cutoff;
      }

      color += clamp(diffuse + specular, 0.0f, 1.0f);
    }
  }

  // Gamma correction.
  // color = pow (color, vec3(1.0 / 2.2)); 

  output_color = vec4(color, 1.0);
}
)";
}