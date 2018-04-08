#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_bindless_texture : enable
layout (bindless_sampler) uniform;
#endif

layout(binding = 0) uniform sampler2D ui_texture;

layout(location = 0) in vs_output_type 
{
  vec2 texture_coordinate;
  vec4 color             ;
} fs_input;

layout(location = 0) out vec4 output_color;

void main()
{
  output_color = fs_input.color * texture(ui_texture, fs_input.texture_coordinate);
}