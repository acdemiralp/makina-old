#include <makina/core/threading/thread_pool.hpp>

namespace mak
{
thread_pool::thread_pool(std::size_t threads)
{
  for (std::size_t i = 0; i < threads; ++i)
    threads_.emplace_back([this]
    {
      for (;;)
      {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(task_queue_mutex_);
          condition_var_.wait(lock, [this] 
          {
            return !running_ || !task_queue_.empty(); 
          });
          if (!running_ && task_queue_.empty())
            return;
          task = std::move(task_queue_.front());
          task_queue_.pop();
        }
        task();
      }
    }
  );
}
thread_pool::~thread_pool()
{
  {
    std::unique_lock<std::mutex> lock(task_queue_mutex_);
    running_ = false;
  }
  condition_var_.notify_all();
  for (auto& worker : threads_)
    worker.join();
}
}