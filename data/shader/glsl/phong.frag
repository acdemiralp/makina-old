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

const float attenuation_constant   = 1.0;
const float attenuation_linear     = 0.01;
const float attenuation_quadratic  = 0.0001;

struct _material
{
  uvec4     use_image       ; // ambient - diffuse - specular - unused
  vec4      ambient         ; // w is unused.
  vec4      diffuse         ; // w is unused.
  vec4      specular        ; // w is shininess.
  uvec4     image_ids       ; // w is unused.
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

void main()
{
  vec3  ka = materials[fs_input.material_index].use_image[0] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[0]))).rgb 
    : materials[fs_input.material_index].ambient.rgb ;             
  vec3  kd = materials[fs_input.material_index].use_image[1] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[1]))).rgb 
    : materials[fs_input.material_index].diffuse.rgb ;             
  vec3  ks = materials[fs_input.material_index].use_image[2] == 1
    ? texture(images, vec3(fs_input.texture_coordinate, float(materials[fs_input.material_index].image_ids[2]))).rgb 
    : materials[fs_input.material_index].specular.rgb;
  float a  = max(materials[fs_input.material_index].specular.w, 0.0001f);
  vec3  n  = normalize( fs_input.normal);
  vec3  v  = normalize(-fs_input.vertex);

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
      vec3  l           = -normalize(cameras[cameras_metadata.y].view * vec4(lights[i].direction.xyz, 0.0)).xyz;
      vec3  r           =  reflect  (-l, n);
      vec3  diffuse     = kd * il * max(dot(n, l), 0.0f);
      vec3  specular    = ks * il * pow(max(dot(v, r), 0.0f), a);
      color += clamp(diffuse + specular, 0.0f, 1.0f);
    }
    if (type == light_type_point || type == light_type_spot)
    {
      vec3  p           = (cameras[cameras_metadata.y].view * vec4(lights[i].position.xyz, 1.0f)).xyz;
      vec3  l           = normalize(p - fs_input.vertex);
      vec3  r           = reflect  (-l, n);
      float distance    = length   (p - fs_input.vertex);
      float attenuation = 1.0 / (attenuation_constant + attenuation_linear * distance + attenuation_quadratic * distance * distance);
      vec3  diffuse     = attenuation * kd * il * max(dot(n, l), 0.0f);
      vec3  specular    = attenuation * ks * il * pow(max(dot(v, r), 0.0f), a);
  
      // Soft-edged spotlights are a special case of point lights.
      if(type == light_type_spot)
      {
        float cutoff = clamp((dot(l, -normalize(cameras[cameras_metadata.y].view * vec4(lights[i].direction.xyz, 0.0f)).xyz) - lights[i].properties.w) / (lights[i].properties.z - lights[i].properties.w), 0.0f, 1.0f);
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