#ifndef MAKINA_RENDERER_BACKEND_GLSL_SHADER_SOURCES_HPP_
#define MAKINA_RENDERER_BACKEND_GLSL_SHADER_SOURCES_HPP_

#include <string>

namespace mak
{
extern std::string default_vertex_shader           ;
extern std::string phong_fragment_shader           ;
extern std::string physically_based_fragment_shader;

extern std::string ui_vertex_shader                ;
extern std::string ui_fragment_shader              ;
}

#endif