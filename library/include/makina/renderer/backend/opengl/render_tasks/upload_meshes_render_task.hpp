#ifndef MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_MESHES_RENDER_TASK_HPP
#define MAKINA_RENDERER_OPENGL_RENDER_TASKS_UPLOAD_MESHES_RENDER_TASK_HPP

#include <cstdint>
#include <map>
#include <vector>

#include <gl/draw_commands.hpp>

#include <makina/renderer/backend/opengl/buffer.hpp>
#include <makina/renderer/backend/opengl/texture.hpp>
#include <makina/renderer/backend/parameter_map.hpp>
#include <makina/renderer/animator.hpp>
#include <makina/renderer/bone.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/export.hpp>

namespace mak
{
namespace opengl
{
struct MAKINA_EXPORT upload_meshes_task_data
{
  buffer_resource*           vertices           ;
  buffer_resource*           normals            ;
  buffer_resource*           colors             ;
  buffer_resource*           texture_coordinates;
  buffer_resource*           bone_ids           ;
  buffer_resource*           bone_weights       ;
  buffer_resource*           instance_attributes;
  buffer_resource*           indices            ;
  buffer_resource*           draw_calls         ;
  buffer_resource*           transforms         ;
  buffer_resource*           materials          ;
  buffer_resource*           rigs               ;
  texture_2d_array_resource* textures           ;
  parameter_map_resource*    parameter_map      ;
                             
  GLuint                     vertex_offset      = 0;
  GLuint                     index_offset       = 0;
  GLuint                     image_offset       = 0;
  GLuint                     rig_offset         = 0;
  
  std::map<mesh*    , gl::draw_elements_indirect_command>        mesh_cache ;
  std::map<image*   , std::uint64_t>                             image_cache;
  std::map<animator*, std::vector<std::pair<transform*, bone*>>> rig_cache  ;
};

MAKINA_EXPORT fg::render_task<upload_meshes_task_data>* add_upload_meshes_render_task(renderer* framegraph);
}
}

#endif