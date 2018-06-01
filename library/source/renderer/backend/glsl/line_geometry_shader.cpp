#include <makina/renderer/backend/glsl/line_geometry_shader.hpp>

namespace mak
{
namespace glsl
{
std::string line_geometry_shader = R"(
#version 450

#ifdef VULKAN

#else
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_KHR_vulkan_glsl : enable
#endif

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

layout(std430, set = 0, binding = 4) readonly buffer _viewport
{
  vec2 viewport;
};

layout(location = 0) in vs_output_type 
{
  vec3                vertex        ;
  vec3                normal        ;
  vec4                color         ;
  noperspective float edge_distance ;
  noperspective float radius        ;
  flat          uint  material_index;
} gs_input[];
layout(location = 0) out vs_output_type 
{
  vec3                vertex        ;
  vec3                normal        ;
  vec4                color         ;
  noperspective float edge_distance ;
  noperspective float radius        ;
  flat          uint  material_index;
} gs_output;

void main()
{
  vec2 position_start      = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
  vec2 position_end        = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
                           
  vec2 direction           = position_end - position_start;
  direction                = normalize(vec2(direction.x, direction.y * viewport.y / viewport.x));
                           
  vec2 tangent_start       = vec2(-direction.y, direction.x);
  vec2 tangent_end         = tangent_start * gs_input[1].radius / viewport;
  tangent_start            = tangent_start * gs_input[0].radius / viewport;
                           
  gl_Position              = vec4((position_start - tangent_start) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw);
  gs_output.vertex         =  gs_input[0].vertex;
  gs_output.color          =  gs_input[0].color ;
  gs_output.edge_distance  = -gs_input[0].radius;
  gs_output.radius         =  gs_input[0].radius;
  gs_output.material_index =  gs_input[0].material_index;
  EmitVertex();

  gl_Position              = vec4((position_start + tangent_start) * gl_in[0].gl_Position.w, gl_in[0].gl_Position.zw);
  gs_output.vertex         =  gs_input[0].vertex;
  gs_output.color          =  gs_input[0].color ;
  gs_output.edge_distance  =  gs_input[0].radius;
  gs_output.radius         =  gs_input[0].radius;
  gs_output.material_index =  gs_input[0].material_index;
  EmitVertex();            
                           
  gl_Position              = vec4((position_end   - tangent_end  ) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw);
  gs_output.vertex         =  gs_input[1].vertex;
  gs_output.color          =  gs_input[1].color ;
  gs_output.edge_distance  = -gs_input[1].radius;
  gs_output.radius         =  gs_input[1].radius;
  gs_output.material_index =  gs_input[0].material_index;
  EmitVertex();            
                           
  gl_Position              = vec4((position_end   + tangent_end  ) * gl_in[1].gl_Position.w, gl_in[1].gl_Position.zw);
  gs_output.vertex         =  gs_input[1].vertex;
  gs_output.color          =  gs_input[1].color ;
  gs_output.edge_distance  =  gs_input[1].radius;
  gs_output.radius         =  gs_input[1].radius;
  gs_output.material_index =  gs_input[0].material_index;
  EmitVertex();
}
)";
}
}