#include <makina/renderer/backend/bgfx/render_tasks/test_render_task.hpp>

#include <bgfx/bgfx.h>
#include <fg/render_task_builder.hpp>

#include <makina/renderer/backend/shaderc/test_shader.hpp>

namespace mak
{
namespace bgfx
{
fg::render_task<test_task_data>* add_test_render_task(renderer* framegraph)
{
  struct vertex
  {
    glm::vec3     position;
    std::uint32_t color   ;
  };
    
  std::vector<vertex> vertices =
  {
    {glm::vec3(-1.0f,  1.0f,  1.0f), 0xff000000},
    {glm::vec3( 1.0f,  1.0f,  1.0f), 0xff0000ff},
    {glm::vec3(-1.0f, -1.0f,  1.0f), 0xff00ff00},
    {glm::vec3( 1.0f, -1.0f,  1.0f), 0xff00ffff},
    {glm::vec3(-1.0f,  1.0f, -1.0f), 0xffff0000},
    {glm::vec3( 1.0f,  1.0f, -1.0f), 0xffff00ff},
    {glm::vec3(-1.0f, -1.0f, -1.0f), 0xffffff00},
    {glm::vec3( 1.0f, -1.0f, -1.0f), 0xffffffff},
  };
  std::vector<std::uint16_t> indices = {0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5};

  auto render_task = framegraph->add_render_task<test_task_data>(
    "BGFX Test Pass",
    [=] (      test_task_data& data, fg::render_task_builder& builder)
    {
      ::bgfx::VertexDecl vertex_declaration;
      vertex_declaration
        .begin()
        .add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float)
        .add(::bgfx::Attrib::Color0  , 4, ::bgfx::AttribType::Uint8, true)
        .end();

      data.vertices = builder.create<buffer_resource> ("BGFX Test Vertices", buffer::description{buffer::description::type::vertex, vertices.size() * sizeof(vertex), vertex_declaration});
      data.indices  = builder.create<buffer_resource> ("BGFX Test Indices" , buffer::description{buffer::description::type::index , indices .size() * sizeof(std::uint16_t)});
      data.program  = builder.create<program_resource>("BGFX Test Program" , program::description{shaderc::test_vertex_shader, shaderc::test_fragment_shader});
    },
    [=] (const test_task_data& data)
    {
      auto camera     = framegraph->scene_cache()->entities<mak::transform, mak::projection>()[0];
      auto view       = inverse(camera->component<mak::transform> ()->matrix(true));
      auto projection = camera->component<mak::projection>()->matrix();
      ::bgfx::setViewTransform(0, &view[0][0], &projection[0][0]);
      
      auto vertices_memory = ::bgfx::copy(vertices.data(), vertices.size() * sizeof(vertex));
      auto indices_memory  = ::bgfx::copy(indices .data(), indices .size() * sizeof(std::uint16_t));
      ::bgfx::updateDynamicVertexBuffer(data.vertices->actual()->native<::bgfx::DynamicVertexBufferHandle>(), 0, vertices_memory);
      ::bgfx::updateDynamicIndexBuffer (data.indices ->actual()->native<::bgfx::DynamicIndexBufferHandle >(), 0, indices_memory );

      ::bgfx::setVertexBuffer (0, data.vertices->actual()->native<::bgfx::DynamicVertexBufferHandle>());
      ::bgfx::setIndexBuffer  (   data.indices ->actual()->native<::bgfx::DynamicIndexBufferHandle> ());
      ::bgfx::setState(0
        | BGFX_STATE_WRITE_R
        | BGFX_STATE_WRITE_G
        | BGFX_STATE_WRITE_B
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_PT_TRISTRIP);
      ::bgfx::submit(0, data.program->actual()->native());
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}