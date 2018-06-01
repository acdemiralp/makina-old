#include <makina/renderer/backend/opengl/render_tasks/upload_lines_render_task.hpp>

#include <makina/renderer/line_render.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/line_segments.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<upload_lines_task_data>* add_upload_lines_render_task(renderer* framegraph)
{
  // Shader types with std430 alignment.
  struct _transform
  {
    glm::mat4 model     ;
  };
  struct _phong_material
  {
    glm::vec4 ambient   ; // w is unused.
    glm::vec4 diffuse   ; // w is unused.
    glm::vec4 specular  ; // w is shininess.
  };
  struct _physically_based_material
  {
    glm::vec4 albedo    ; // w is unused.
    glm::vec4 properties; // metallicity - roughness - unused - unused
  };

  const auto retained_vertices            = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Vertices"           , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_colors              = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Colors"             , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_instance_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Instance Attributes", buffer_description{GLsizeiptr(64e+6)});
  const auto retained_indices             = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Indices"            , buffer_description{GLsizeiptr(64e+6)});
  const auto retained_draw_calls          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Draw Calls"         , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_transforms          = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Transforms"         , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_materials           = framegraph->add_retained_resource<buffer_description, gl::buffer>           ("Line Materials"          , buffer_description{GLsizeiptr(16e+6)});
  const auto retained_parameter_map       = framegraph->add_retained_resource<parameter_map::description, parameter_map>("Line Parameter Map"      , parameter_map::description());

  return framegraph->add_render_task<upload_lines_task_data>(
    "Upload Lines Pass",
    [=] (      upload_lines_task_data& data, fg::render_task_builder& builder)
    {
      data.vertices            = builder.write<buffer_resource>       (retained_vertices           );
      data.colors              = builder.write<buffer_resource>       (retained_colors             );
      data.instance_attributes = builder.write<buffer_resource>       (retained_instance_attributes);
      data.indices             = builder.write<buffer_resource>       (retained_indices            );
      data.draw_calls          = builder.write<buffer_resource>       (retained_draw_calls         );
      data.transforms          = builder.write<buffer_resource>       (retained_transforms         );
      data.materials           = builder.write<buffer_resource>       (retained_materials          );
      data.parameter_map       = builder.write<parameter_map_resource>(retained_parameter_map      );
    },
    [=] (const upload_lines_task_data& data)
    {
      auto& mutable_data         = const_cast<upload_lines_task_data&>(data);
                                
      auto  scene                = framegraph->scene_cache();
      auto  line_render_entities = scene->entities<transform, line_render>        ();
      auto  instance_attributes  = std::vector<glm::uvec2>                        ();
      auto  draw_calls           = std::vector<gl::draw_elements_indirect_command>();
      auto  transforms           = std::vector<_transform>                        ();
      auto  pbr_materials        = std::vector<_physically_based_material>        ();
      auto  phong_materials      = std::vector<_phong_material>                   ();
      
      for (auto i = 0; i < line_render_entities.size(); ++i)
      {
        const auto& entity      = line_render_entities[i];
        const auto  metadata    = entity->component<mak::metadata>   ();
        const auto  transform   = entity->component<mak::transform>  ();
        const auto  line_render = entity->component<mak::line_render>();
        
        if (!metadata->active) continue;

        transforms.push_back(_transform {transform->matrix(true)});

        auto pbr_material   = dynamic_cast<mak::physically_based_material*>(line_render->material);
        if  (pbr_material)
          pbr_materials.push_back(_physically_based_material {
            glm::vec4 (pbr_material->albedo, 0.0f),
            glm::vec4 (pbr_material->metallicity, pbr_material->roughness, 0.0f, 0.0f)});
        auto phong_material = dynamic_cast<mak::phong_material*>           (line_render->material);
        if  (phong_material)
          phong_materials .push_back(_phong_material {
            glm::vec4(phong_material->ambient , 0.0f),
            glm::vec4(phong_material->diffuse , 0.0f),
            glm::vec4(phong_material->specular, phong_material->shininess)});

        if (!mutable_data.line_segments_cache.count(line_render->line_segments))
        {
          const auto& vertices = line_render->line_segments->vertices;
          const auto& colors   = line_render->line_segments->colors  ;
          const auto& indices  = line_render->line_segments->indices ;
          const auto& radius   = line_render->line_segments->radius  ;
          
          std::vector<glm::vec4> transformed_vertices (vertices.size());
          std::transform(vertices.begin(), vertices.end  (), transformed_vertices.begin(), [radius] (const glm::vec3&  vertex) { return glm::vec4(vertex, radius); });
          
          data.vertices->actual()->set_sub_data(sizeof transformed_vertices[0] * mutable_data.vertex_offset, sizeof transformed_vertices[0] * transformed_vertices.size(), transformed_vertices.data());
          data.colors  ->actual()->set_sub_data(sizeof colors              [0] * mutable_data.vertex_offset, sizeof colors              [0] * colors              .size(), colors              .data());
          data.indices ->actual()->set_sub_data(sizeof indices             [0] * mutable_data.index_offset , sizeof indices             [0] * indices             .size(), indices             .data());

          mutable_data.line_segments_cache[line_render->line_segments] = gl::draw_elements_indirect_command { static_cast<GLuint>(indices.size()), 1, mutable_data.index_offset, mutable_data.vertex_offset, 0 };
          
          mutable_data.vertex_offset += static_cast<GLuint>(vertices.size());
          mutable_data.index_offset  += static_cast<GLuint>(indices .size());
        }

        auto draw_call          = mutable_data.line_segments_cache[line_render->line_segments];
        draw_call.base_instance = i;
        instance_attributes.push_back({i, i});
        draw_calls         .push_back(draw_call);
      }

      auto transforms_metadata = glm::uvec4(transforms   .size(),                                                     0u, 0u, 0u);
      auto materials_metadata  = glm::uvec4(pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size(), 0u, 0u, 0u);
      data.instance_attributes->actual()->set_sub_data(0                         , sizeof instance_attributes[0] * instance_attributes.size(), instance_attributes .data());
      data.draw_calls         ->actual()->set_sub_data(0                         , sizeof draw_calls         [0] * draw_calls         .size(), draw_calls          .data());
      data.transforms         ->actual()->set_sub_data(0                         , sizeof transforms_metadata                                , &transforms_metadata       );
      data.transforms         ->actual()->set_sub_data(sizeof transforms_metadata, sizeof transforms         [0] * transforms         .size(), transforms          .data());
      data.materials          ->actual()->set_sub_data(0                         , sizeof materials_metadata                                 , &materials_metadata        );
      pbr_materials.size() > 0                                                                                                                                     
      ? data.materials        ->actual()->set_sub_data(sizeof materials_metadata , sizeof pbr_materials      [0] * pbr_materials      .size(), pbr_materials       .data())
      : data.materials        ->actual()->set_sub_data(sizeof materials_metadata , sizeof phong_materials    [0] * phong_materials    .size(), phong_materials     .data());
      
      data.parameter_map->actual()->set("vertex_count"   , mutable_data.vertex_offset);
      data.parameter_map->actual()->set("index_count"    , mutable_data.index_offset );
      data.parameter_map->actual()->set("draw_count"     , draw_calls   .size());
      data.parameter_map->actual()->set("transform_count", transforms   .size());
      data.parameter_map->actual()->set("material_count" , pbr_materials.size() > 0 ? pbr_materials.size() : phong_materials.size());

      gl::print_error("Error in Upload Lines Pass");
    });
}
}
}
