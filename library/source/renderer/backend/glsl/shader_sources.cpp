#include <makina/renderer/backend/glsl/shader_sources.hpp>

namespace mak
{
std::string test_vertex_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in  vec3 vertex;
layout(location = 0) out vec3 color ;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
}
#endif

void main() 
{
  color       = vertex           ;
  gl_Position = vec4(vertex, 1.0);
}
)";
std::string test_fragment_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in  vec3 color     ;
layout(location = 0) out vec4 frag_color;

void main() 
{
  frag_color = vec4(color.x + 0.5f, 0.5f, color.y + 0.5f, 1.0f);
}
)";

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

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
}
#endif

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
std::string physically_based_fragment_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;

const uint  light_type_ambient     = 0;
const uint  light_type_directional = 1;
const uint  light_type_point       = 2;
const uint  light_type_spot        = 3;
const float pi                     = 3.14159265359;

struct _material
{
  uvec4     use_texture              ; // albedo - metallicity - roughness - normal
  uvec4     use_texture_2            ; // ambient occlusion - unused - unused - unused
  vec4      albedo                   ; // w is unused.
  vec4      properties               ; // metallicity - roughness - unused - unused
  sampler2D albedo_texture           ;
  sampler2D metallicity_texture      ;
  sampler2D roughness_texture        ;
  sampler2D normal_texture           ;
  sampler2D ambient_occlusion_texture;
  sampler2D unused_texture_1         ;
  sampler2D unused_texture_2         ;
  sampler2D unused_texture_3         ;
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
  uint      materials_size;
  _material materials[]   ;
};
layout(std430, binding = 2) readonly buffer camera
{
  uint      cameras_size  ;
  _camera   cameras[]     ;
};
layout(std430, binding = 3) readonly buffer light
{
  uint      lights_size   ;
  _light    lights[]      ;
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
vec3  sample_normal_texture(vec3 vertex, vec3 normal, vec2 texture_coordinate, sampler2D normal_texture)
{
  vec3 s   = texture(normal_texture, texture_coordinate).xyz * 2.0f - 1.0f;

  vec3 q1  = dFdx(vertex);
  vec3 q2  = dFdy(vertex);
  vec2 t1  = dFdx(texture_coordinate);
  vec2 t2  = dFdy(texture_coordinate);
           
  vec3 n   = normalize(normal);
  vec3 t   = normalize(q1 * t2.t - q2 * t1.t);
  vec3 b   = -normalize(cross(n, t));
  mat3 tbn = mat3(t, b, n);

  return normalize(tbn * s);
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
  vec3  ka  = materials[fs_input.material_index].use_texture  [0] == 1
    ? texture(materials[fs_input.material_index].albedo_texture     , fs_input.texture_coordinate).rgb 
    : materials[fs_input.material_index].albedo.rgb  ;             
  float km  = materials[fs_input.material_index].use_texture  [1] == 1
    ? texture(materials[fs_input.material_index].metallicity_texture, fs_input.texture_coordinate).r 
    : materials[fs_input.material_index].properties.x;             
  float kr  = materials[fs_input.material_index].use_texture  [2] == 1
    ? texture(materials[fs_input.material_index].roughness_texture  , fs_input.texture_coordinate).r 
    : materials[fs_input.material_index].properties.y; 
  vec3  n   = materials[fs_input.material_index].use_texture  [3] == 1
    ? sample_normal_texture(fs_input.vertex, fs_input.normal, fs_input.texture_coordinate, materials[fs_input.material_index].normal_texture)
    : normalize(fs_input.normal); 
  vec3  kao = materials[fs_input.material_index].use_texture_2[0] == 1
    ? texture(materials[fs_input.material_index].ambient_occlusion_texture, fs_input.texture_coordinate).rgb 
    : vec3(0.0f);

  vec3 v = normalize(-fs_input.vertex);

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
      vec3 l = -normalize(cameras[camera_index].view * vec4(lights[i].direction.xyz, 0.0f)).xyz;
      color += shade_ggx(v, n, l, ka, km, kr, il);
    }
    if (type == light_type_point || type == light_type_spot)
    {
      vec3 p = (cameras[camera_index].view * vec4(lights[i].position.xyz, 1.0f)).xyz;
      vec3 l = normalize(p - fs_input.vertex);
    
      float distance    = length(p - fs_input.vertex);
      float attenuation = 1.0 / (attenuation_constant + attenuation_linear * distance + attenuation_quadratic * distance * distance);
      float cutoff      = type == light_type_spot 
        ? clamp((dot(l, -normalize(cameras[camera_index].view * vec4(lights[i].direction.xyz, 0.0f)).xyz) - lights[i].properties.w) / (lights[i].properties.z - lights[i].properties.w), 0.0f, 1.0f) 
        : 1.0f;
      
      color += attenuation * cutoff * shade_ggx(v, n, l, ka, km, kr, il);
    }
  }

  // Gamma correction.
  color = pow (color, vec3(1.0 / 2.2)); 

  output_color = vec4(color, 1.0);
}
)";

std::string ui_vertex_shader   = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable

uniform mat4 projection;

layout(location = 0) in vec2 vertex            ;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in vec4 color             ;

out vs_output_type 
{
  vec2 texture_coordinate;
  vec4 color             ;
} vs_output;

#ifdef VULKAN
out gl_PerVertex
{
  vec4 gl_Position;
}
#endif

void main()
{
  vs_output.texture_coordinate = texture_coordinate;
  vs_output.color              = color             ;
  gl_Position                  = projection * vec4(vertex, 0.0f, 1.0f);
}
)";
std::string ui_fragment_shader = R"(
#version 450
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;

uniform sampler2D ui_texture;

in vs_output_type 
{
  vec2 texture_coordinate;
  vec4 color             ;
} fs_input;

layout(location = 0) out vec4 output_color;

void main()
{
  output_color = fs_input.color * texture(ui_texture, fs_input.texture_coordinate);
}
)";
}