#include <makina/renderer/backend/glsl/immediate_lines_geometry_shader.hpp>

namespace mak
{
namespace glsl
{
std::string immediate_lines_geometry_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

const float antialiasing = 2.0f;

layout(std430, set = 0, binding = 1) readonly buffer _viewport
{
  vec2 viewport;
};

layout(location = 0) in vs_output_type 
{
  noperspective float edge_distance;
  noperspective float size         ;
  vec4                color        ;
} gs_input[];
layout(location = 0) out vs_output_type 
{
  noperspective float edge_distance;
  noperspective float size         ;
  vec4                color        ;
} gs_output;

void main()
{
  vec2 position_start     = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
  vec2 position_end       = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
  
  vec2 direction          = position_start - position_end;
  direction               = normalize(vec2(direction.x, direction.y * viewport.y / viewport.x));

  vec2 tangent_start      = vec2(-direction.y, direction.x);
  vec2 tangent_end        = tangent_start * gs_input[1].size / viewport;
  tangent_start           = tangent_start * gs_input[0].size / viewport;

  gl_Position             = vec4((position_start - tangent_start) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw);
  gs_output.edge_distance = -gs_input[0].size;
  gs_output.size          =  gs_input[0].size;
  gs_output.color         =  gs_input[0].color;
  EmitVertex();

  gl_Position             = vec4((position_start + tangent_start) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw);
  gs_output.edge_distance =  gs_input[0].size;
  gs_output.size          =  gs_input[0].size;
  gs_output.color         =  gs_input[0].color;
  EmitVertex();
  
  gl_Position             = vec4((position_end   - tangent_end  ) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw);
  gs_output.edge_distance = -gs_input[0].size;
  gs_output.size          =  gs_input[0].size;
  gs_output.color         =  gs_input[0].color;
  EmitVertex();

  gl_Position             = vec4((position_end   + tangent_end  ) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw);
  gs_output.edge_distance =  gs_input[0].size;
  gs_output.size          =  gs_input[0].size;
  gs_output.color         =  gs_input[0].color;
  EmitVertex();
}
)";
}
}