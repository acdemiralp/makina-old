#include <makina/renderer/backend/opengl/render_tasks/immediate_render_task.hpp>

#include <gl/draw_commands.hpp>
#include <gl/per_fragment_ops.hpp>
#include <gl/rasterization.hpp>
#include <gl/viewport.hpp>
#include <im3d.h>

#include <makina/renderer/backend/glsl/immediate_lines_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_lines_geometry_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_lines_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_points_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_points_vertex_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_triangles_fragment_shader.hpp>
#include <makina/renderer/backend/glsl/immediate_triangles_vertex_shader.hpp>

namespace mak
{
namespace opengl
{
fg::render_task<immediate_task_data>* add_immediate_render_task (renderer* framegraph, framebuffer_resource* target, const upload_scene_task_data& scene_data, const std::string& camera_tag)
{
  const auto retained_attributes = framegraph->add_retained_resource<buffer_description, gl::buffer>("Immediate Vertices"  , buffer_description{GLsizeiptr(4e+6) , GL_ARRAY_BUFFER         });
  const auto retained_viewport   = framegraph->add_retained_resource<buffer_description, gl::buffer>("Immediate Viewport"  , buffer_description{sizeof(glm::vec2), GL_SHADER_STORAGE_BUFFER});

  return framegraph->add_render_task<immediate_task_data>(
    "Immediate Pass",
    [&] (      immediate_task_data& data, fg::render_task_builder& builder)
    {
      data.attributes        = builder.read  <buffer_resource>          (retained_attributes);
      data.cameras           = builder.read  <buffer_resource>          (scene_data.cameras );
      data.viewport          = builder.read  <buffer_resource>          (retained_viewport  );
      data.points_program    = builder.create<graphics_program_resource>("Immediate Points Program"   , program::graphics_description     
      {
        glsl::immediate_points_vertex_shader, 
        glsl::immediate_points_fragment_shader
      });
      data.lines_program     = builder.create<graphics_program_resource>("Immediate Lines Program"    , program::graphics_description     
      {
        glsl::immediate_lines_vertex_shader,
        glsl::immediate_lines_fragment_shader,
        glsl::immediate_lines_geometry_shader
      });
      data.triangles_program = builder.create<graphics_program_resource>("Immediate Triangles Program", program::graphics_description     
      {
        glsl::immediate_triangles_vertex_shader, 
        glsl::immediate_triangles_fragment_shader
      });
      data.vertex_array      = builder.create<vertex_array_resource>    ("Immediate Vertex Array"     , vertex_array::description
      {
        { 
          {data.attributes, 4, GL_FLOAT        , false, 0, sizeof Im3d::VertexData, offsetof(Im3d::VertexData, m_positionSize)},
          {data.attributes, 4, GL_UNSIGNED_BYTE, true , 0, sizeof Im3d::VertexData, offsetof(Im3d::VertexData, m_color       )}
        }, 
        {
          data.cameras,
          data.viewport
        }
      });
      data.target            = builder.write(target);

      data.draw_callback     = [&] (const Im3d::DrawList& draw_list)
      {
        auto& app_data = Im3d::GetAppData();

        GLenum primitive_type;
        if      (draw_list.m_primType == Im3d::DrawPrimitive_Points) primitive_type = GL_POINTS   ;
        else if (draw_list.m_primType == Im3d::DrawPrimitive_Lines ) primitive_type = GL_LINES    ;
        else                                                         primitive_type = GL_TRIANGLES;
        
        if(!camera_tag.empty())
        {
          auto cameras = framegraph->scene_cache()->entities<metadata, projection>();
          for (std::uint32_t i = 0; i < cameras.size(); ++i)
            if (cameras[i]->component<metadata>()->contains_tag(camera_tag))
              data.cameras->actual()->set_sub_data(sizeof glm::uint, sizeof glm::uint, &i);
        }

        glm::vec2 size {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()};

        data.attributes->actual()->set_sub_data(0, draw_list.m_vertexCount * sizeof Im3d::VertexData, draw_list.m_vertexData);
        data.viewport  ->actual()->set_sub_data(0, sizeof glm::vec2, &size[0]);

        data.vertex_array->actual()->bind();
        if      (primitive_type == GL_POINTS) data.points_program   ->actual()->use  ();
        else if (primitive_type == GL_LINES ) data.lines_program    ->actual()->use  ();
        else                                  data.triangles_program->actual()->use  ();

        gl::set_viewport          ({0, 0}, {data.target->actual()->color_texture()->width(), data.target->actual()->color_texture()->height()});
        gl::set_point_size_enabled(true);
        gl::set_blending_enabled  (true);
        gl::set_blend_equation    (GL_FUNC_ADD , GL_FUNC_ADD );
        gl::set_blend_function    (GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gl::draw_arrays           (primitive_type, 0, draw_list.m_vertexCount);

        if      (primitive_type == GL_POINTS) data.points_program   ->actual()->unuse();
        else if (primitive_type == GL_LINES ) data.lines_program    ->actual()->unuse();
        else                                  data.triangles_program->actual()->unuse();
        data.vertex_array->actual()->unbind();
      };

      Im3d::GetAppData().m_appData = &data.draw_callback;
      Im3d::GetAppData().drawCallback = [ ] (const Im3d::DrawList& draw_list)
      {
        auto& closure = *reinterpret_cast<std::function<void(const Im3d::DrawList&)>*>(Im3d::GetAppData().m_appData);
        closure(draw_list);
      };
    },
    [=] (const immediate_task_data& data)
    {
      auto& app_data = Im3d::GetAppData();

      app_data.m_deltaTime = 

      //auto& io = ImGui::GetIO();
      //
      //vertex_array->set_vertex_buffer   (0, *data.attributes->actual(), 0, sizeof ImDrawVert);
      //vertex_array->set_vertex_buffer   (1, *data.attributes->actual(), 0, sizeof ImDrawVert);
      //vertex_array->set_vertex_buffer   (2, *data.attributes->actual(), 0, sizeof ImDrawVert);
      //vertex_array->set_attribute_format(0, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, pos));
      //vertex_array->set_attribute_format(1, 2, GL_FLOAT        , false, IM_OFFSETOF(ImDrawVert, uv ));
      //vertex_array->set_attribute_format(2, 4, GL_UNSIGNED_BYTE, true , IM_OFFSETOF(ImDrawVert, col));
      //
      //ImGui::Render();
      //auto draw_data = ImGui::GetDrawData();
      //for(auto i = 0; i < draw_data->CmdListsCount; ++i)
      //{
      //  const ImDrawList* command_list = draw_data->CmdLists[i];
      //  const ImDrawIdx*  index_offset = nullptr;
      //  data.attributes->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->VtxBuffer.Size) * sizeof(ImDrawVert), static_cast<const GLvoid*>(command_list->VtxBuffer.Data));
      //  data.indices   ->actual()->set_sub_data(0, static_cast<GLsizeiptr>(command_list->IdxBuffer.Size) * sizeof(ImDrawIdx) , static_cast<const GLvoid*>(command_list->IdxBuffer.Data));
      //  for(auto j = 0; j < command_list->CmdBuffer.Size; ++j)
      //  {
      //    auto& command = command_list->CmdBuffer[j];
      //    gl::set_scissor(
      //      {static_cast<int>(command.ClipRect.x)                     , static_cast<int>(io.DisplaySize.y   - command.ClipRect.w)},
      //      {static_cast<int>(command.ClipRect.z - command.ClipRect.x), static_cast<int>(command.ClipRect.w - command.ClipRect.y)});
      //    gl::draw_elements(GL_TRIANGLES, static_cast<GLsizei>(command.ElemCount), GL_UNSIGNED_SHORT, index_offset);
      //    index_offset += command.ElemCount;
      //  }
      //}

      Im3d::NewFrame();

      gl::print_error("Error in Immediate Pass");
    });
}
}
}
