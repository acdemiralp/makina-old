#ifndef MAKINA_CORE_THREADING_TASK_GRAPH_HPP_
#define MAKINA_CORE_THREADING_TASK_GRAPH_HPP_

#include <functional>

#include <boost/graph/directed_graph.hpp>

#include <export.hpp>

namespace mak
{
class MAKINA_EXPORT task_graph : public boost::directed_graph<std::function<void()>>
{
public:
  task_graph()                        = default;
  task_graph(const task_graph&  that) = delete ;
  task_graph(      task_graph&& temp) = default;
  virtual ~task_graph()               = default;

  task_graph& operator=(const task_graph&  that) = delete ;
  task_graph& operator=(      task_graph&& temp) = default;

  void traverse()
  {
    
  }

private:

};
}

#endif