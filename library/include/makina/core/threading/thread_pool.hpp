#ifndef MAKINA_CORE_THREADING_THREAD_POOL_HPP_
#define MAKINA_CORE_THREADING_THREAD_POOL_HPP_

#include <condition_variable>
#include <future>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

#include <makina_export.hpp>

namespace mak
{
class MAKINA_EXPORT thread_pool final
{
public:
  thread_pool(size_t thread_count);
 ~thread_pool();

  template<class function, class... arguments>
  std::future<typename std::result_of<function(arguments...)>::type> enqueue(function&& func, arguments&&... args);

private:
  std::vector<std::thread>           threads_         ;
  std::queue <std::function<void()>> task_queue_      ;
  std::mutex                         task_queue_mutex_;
  std::condition_variable            condition_var_   ;
  bool                               running_         = false;
};

template<class function, class... arguments>
std::future<typename std::result_of<function(arguments...)>::type> thread_pool::enqueue(function&& func, arguments&&... args)
{
  using return_type = typename std::result_of<function(arguments...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<function>(func), std::forward<arguments>(args)...));
  std::future<return_type> result = task->get_future();
  {
    std::unique_lock<std::mutex> lock(task_queue_mutex_);
    if (!running_)
      throw std::runtime_error("Cannot enqueue: Thread pool is pending destruction.");
    task_queue_.emplace([task]()
    {
      (*task)();
    });
  }
  condition_var_.notify_one();
  return result;
}
}

#endif