#include <iostream>

#include <glm/glm.hpp>

#include <makina/core/engine.hpp>
#include <makina/core/rendering/backends/opengl/all.hpp>
#include <makina/core/rendering/framegraph/framegraph.hpp>
#include <makina/core/rendering/framegraph/render_pass.hpp>
#include <makina/core/rendering/framegraph/render_pass_resources.hpp>
#include <makina/core/rendering/low_level_renderer/render_context.hpp>
#include <makina/core/rendering/resources/resource_builder.hpp>
#include <makina/core/rendering/resources/resource_registry.hpp>
#include <makina/core/threading/task_graph.hpp>
#include <makina/systems/display/display_system.hpp>

int main(int argc, char** argv)
{
  mak::engine engine;
  engine.add_system<mak::display_system>();
  
  /*
  std::vector<glm::vec4> vertices = {
    {1.0, 1.0, 1.0, 1.0}, 
    {2.0, 2.0, 2.0, 2.0}, 
    {3.0, 3.0, 3.0, 3.0}, 
    {4.0, 4.0, 4.0, 4.0}};

  mak::gl::shader vs(glCreateShader(GL_VERTEX_SHADER), mak::gl::unmanaged);
  mak::gl::shader fs(GL_FRAGMENT_SHADER);
  vs.set_source("test");
  fs.set_source("test");

  mak::gl::program program;
  program.attach_shader(vs);

  mak::gl::buffer buffer;
  buffer.set_data(vertices.size() * sizeof(glm::vec4), vertices.data());

  mak::gl::texture_2d texture_2d;
  texture_2d.set_storage  (0, GL_RGBA, 32, 32);
  texture_2d.set_sub_image(0, 0, 0, 32, 32, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  mak::gl::query<GL_TIME_ELAPSED> query;
  query.begin  ();
  query.end    ();
  query.counter();

  mak::gl::query<GL_SAMPLES_PASSED> query_2;
  query_2.begin_conditional_render();

  mak::gl::set_seamless_cubemap_enabled(true);
  */

  mak::task_graph task_graph;
  auto task1 = task_graph.add_vertex([&]()
  {
    std::cout << "Task 1." << std::endl;
  });
  auto task2 = task_graph.add_vertex([&]()
  {
    std::cout << "Task 2." << std::endl;
  });
  auto task3 = task_graph.add_vertex([&]()
  {
    std::cout << "Task 3." << std::endl;
  });
  task_graph.add_edge(task1, task2);
  task_graph.add_edge(task2, task3);
  task_graph.traverse();

  class test_resource : public mak::resource
  {
  public:
    class description : public mak::resource::description
    {

    };
  };
  class test_data
  {

  };
  class test_data_2
  {

  };

  mak::resource_registry registry;
  mak::resource_builder  builder (&registry);
  mak::render_context    context ;
  mak::frame_graph       frame_graph(registry, builder, &context);
  frame_graph.add_render_pass<test_data>(
    "Test Pass 1", 
    [](      test_data& data,       mak::resource_builder&      resource_builder)
    {
      resource_builder.create<test_resource>(test_resource::description());
    },
    [](const test_data& data, const mak::render_pass_resources& resources, const mak::render_context* render_context)
    {
      //resource_registry.get_resource<test_resource>(0);
    });
  frame_graph.add_render_pass<test_data_2>(
    "Test Pass 2", 
    [](      test_data_2& data,       mak::resource_builder&  resource_builder)
    {
      
    },
    [](const test_data_2& data, const mak::render_pass_resources& resources, const mak::render_context* render_context)
    {

    });
  frame_graph.traverse();

  engine.run();
  return 0;
}
