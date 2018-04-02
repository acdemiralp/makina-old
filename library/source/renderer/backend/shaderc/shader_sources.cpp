#include <makina/renderer/backend/shaderc/shader_sources.hpp>

namespace mak
{
namespace shaderc
{
std::string test_varying_definitions = 
R"(
vec3 a_position : POSITION ;
vec4 a_color0   : COLOR0   ;

vec4 v_color0   : COLOR0   = vec4(1.0, 0.0, 0.0, 1.0);
)";

std::string test_vertex_shader = 
R"(
$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

void main()
{
  gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
  v_color0    = a_color0;
}
)";
std::string test_fragment_shader = 
R"(
$input v_color0

#include <bgfx_shader.sh>

void main()
{
  gl_FragColor = v_color0;
}
)";
}
}