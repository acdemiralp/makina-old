#include <makina/renderer/backend/bgfx/render_tasks.hpp>

#include <makina/renderer/backend/shaderc/shader_sources.hpp>

namespace mak
{
namespace bgfx
{
fg::render_task<test_task_data>*    add_test_render_task   (renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<test_task_data>(
    "BGFX Test Pass",
    [ ] (      test_task_data& data, fg::render_task_builder& builder)
    {
      ::bgfx::Memory        vertex_shader   {reinterpret_cast<std::uint8_t*>(shaderc::test_vertex_shader  .data()), shaderc::test_vertex_shader  .size()};
      ::bgfx::Memory        fragment_shader {reinterpret_cast<std::uint8_t*>(shaderc::test_fragment_shader.data()), shaderc::test_fragment_shader.size()};
      ::bgfx::ShaderHandle  vertex_handle   = ::bgfx::createShader (&vertex_shader  );
      ::bgfx::ShaderHandle  fragment_handle = ::bgfx::createShader (&fragment_shader);
      ::bgfx::ProgramHandle program         = ::bgfx::createProgram(vertex_handle, fragment_handle, true);
    },
    [ ] (const test_task_data& data)
    {
    });
  render_task->set_cull_immune(true);
  return render_task;
}
fg::render_task<present_task_data>* add_present_render_task(renderer* framegraph)
{
  auto render_task = framegraph->add_render_task<present_task_data>(
    "BGFX Present Pass",
    [ ] (      present_task_data& data, fg::render_task_builder& builder)
    {
    
    },
    [ ] (const present_task_data& data)
    {
      ::bgfx::frame() ;
      ::bgfx::touch(0);
    });
  render_task->set_cull_immune(true);
  return render_task;
}
}
}