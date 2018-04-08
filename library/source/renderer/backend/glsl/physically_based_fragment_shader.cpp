#include <makina/renderer/backend/glsl/physically_based_fragment_shader.hpp>

namespace mak
{
namespace glsl
{
std::string physically_based_fragment_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;
#endif

const uint  light_type_ambient     = 0;
const uint  light_type_directional = 1;
const uint  light_type_point       = 2;
const uint  light_type_spot        = 3;

const float pi                     = 3.14159265359;
const float attenuation_constant   = 1.0;
const float attenuation_linear     = 0.01;
const float attenuation_quadratic  = 0.0001;

struct _material
{
  uvec4     use_image                ; // albedo - metallicity - roughness - normal
  uvec4     use_image_2              ; // ambient occlusion - unused - unused - unused
  vec4      albedo                   ; // w is unused.
  vec4      properties               ; // metallicity - roughness - unused - unused
  uvec4     image_ids                ; // albedo - metallicity - roughness - normal
  uvec4     image_ids_2              ; // ambient occlusion - unused - unused - unused
};
struct _camera
{
  mat4      view                     ;
  mat4      projection               ;
};                                   
struct _light                        
{                                    
  uvec4     type                     ; // y, z, w are unused. 
  vec4      color                    ; // w is unused.
  vec4      properties               ; // intensity - range - inner spot - outer spot
  vec4      position                 ; // w is unused.
  vec4      direction                ; // w is unused.
};                                   

layout(std430, binding = 1) readonly buffer material
{
  uvec4     materials_metadata; // x size
  _material materials[]       ;
};
layout(std430, binding = 2) readonly buffer camera
{
  uvec4     cameras_metadata  ; // x size, y index
  _camera   cameras[]         ;
};
layout(std430, binding = 3) readonly buffer light
{
  uvec4     lights_metadata   ; // x size
  _light    lights[]          ;
};

layout(binding = 0) uniform sampler2DArray images;

layout(location = 0) in vs_output_type 
{
  vec3      vertex            ;
  vec3      normal            ;
  vec2      texture_coordinate;
  flat uint material_index    ;
} fs_input;

layout(location = 0) out vec4 output_color;

float distribution_ggx     (float ndh,            float roughness)
{
  float a    = pow(roughness, 2);
  float a2   = pow(a        , 2);
  float ndh2 = pow(ndh      , 2);
  return a2 / (pi * pow(ndh2 * (a2 - 1.0f) + 1.0f, 2));
}
float geometry_schlick_ggx (float ndv,            float roughness)
{
  float k = pow(roughness + 1.0f, 2) / 8.0f;
  return ndv / (ndv * (1.0f - k) + k);
}
float geometry_smith       (float ndv, float ndl, float roughness)
{
  return geometry_schlick_ggx(ndv, roughness) * geometry_schlick_ggx(ndl, roughness);
}
vec3  fresnel_schlick      (float hdv, vec3  f0)
{
  return mix(vec3(pow(1.0f - hdv, 5.0f)), vec3(1.0), f0);
}
vec3  apply_mapped_normal  (vec3 vertex, vec3 normal, vec2 texture_coordinate, vec3 mapped_normal)
{
  vec3 q1  = dFdx(vertex);
  vec3 q2  = dFdy(vertex);
  vec2 t1  = dFdx(texture_coordinate);
  vec2 t2  = dFdy(texture_coordinate);
           
  vec3 n   = normalize(normal);
  vec3 t   = normalize(q1 * t2.t - q2 * t1.t);
  vec3 b   = -normalize(cross(n, t));
  mat3 tbn = mat3(t, b, n);

  return normalize(tbn * mapped_normal);
}
vec3   shade_ggx           (vec3 v, vec3 n, vec3 l, vec3 ka, float km, float kr, vec3 il)
{
  vec3  h   = normalize(v + l);

  float ndv = max(dot(n, v), 0.0);
  float ndl = max(dot(n, l), 0.0f);
  float ndh = max(dot(n, h), 0.0f);
  float ldh = max(dot(l, h), 0.0f);

  float d   = distribution_ggx(ndh, kr);
  float g   = geometry_smith  (ndv, ndl, kr);
  vec3  f   = fresnel_schlick (ldh, vec3(0.04f)); // TODO: Consider computing k0 from a refraction index.

  vec3  w   = (vec3(1.0f) - f) * (1.0f - km);
  vec3  s   = (d * g * f) / (4.0f * ndv * ndl + 0.0001f);

  return clamp(ndl * il * (w * ka / pi + s), 0.0f, 1.0f);
}

void main()
{
  vec3  ka  = materials[fs_input.material_index].use_image  [0] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[0]))).rgb 
    : materials[fs_input.material_index].albedo.rgb  ;             
  float km  = materials[fs_input.material_index].use_image  [1] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[1]))).r 
    : materials[fs_input.material_index].properties.x;             
  float kr  = materials[fs_input.material_index].use_image  [2] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[2]))).r 
    : materials[fs_input.material_index].properties.y; 
  vec3  n   = materials[fs_input.material_index].use_image  [3] == 1
    ? apply_mapped_normal(fs_input.vertex, fs_input.normal, fs_input.texture_coordinate, texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[3]))).xyz * 2.0f - 1.0f)
    : normalize(fs_input.normal); 
  vec3  kao = materials[fs_input.material_index].use_image_2[0] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids_2[0]))).rgb
    : vec3(0.0f);

  vec3 v = normalize(-fs_input.vertex);

  vec3 color = vec3(0.0f);
  
  for(uint i = 0; i < lights_metadata.x; ++i)
  {
    vec3 il   = lights[i].properties.x * lights[i].color.rgb;
    uint type = lights[i].type.x;
  
    if (type == light_type_ambient)
    {
      color += clamp(vec3(ka * il), 0.0f, 1.0f);
    }
    if (type == light_type_directional)
    {
      vec3 l = -normalize(cameras[cameras_metadata.y].view * vec4(lights[i].direction.xyz, 0.0f)).xyz;
      color += shade_ggx(v, n, l, ka, km, kr, il);
    }
    if (type == light_type_point || type == light_type_spot)
    {
      vec3 p = (cameras[cameras_metadata.y].view * vec4(lights[i].position.xyz, 1.0f)).xyz;
      vec3 l = normalize(p - fs_input.vertex);
    
      float distance    = length(p - fs_input.vertex);
      float attenuation = 1.0 / (attenuation_constant + attenuation_linear * distance + attenuation_quadratic * distance * distance);
      float cutoff      = type == light_type_spot 
        ? clamp((dot(l, -normalize(cameras[cameras_metadata.y].view * vec4(lights[i].direction.xyz, 0.0f)).xyz) - lights[i].properties.w) / (lights[i].properties.z - lights[i].properties.w), 0.0f, 1.0f) 
        : 1.0f;
      
      color += attenuation * cutoff * shade_ggx(v, n, l, ka, km, kr, il);
    }
  }

  // Gamma correction.
  color = pow (color, vec3(1.0 / 2.2)); 

  output_color = vec4(color, 1.0);
}
)";
}
}