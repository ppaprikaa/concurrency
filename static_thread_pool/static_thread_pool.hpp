#pragma once
#include "blocking_queue.hpp"
#include "task.hpp"
#include <thread>
#include <vector>

namespace tp {
class static_thread_pool {
public:
  static_thread_pool(size_t workers) { start_worker_threads(workers); }

  void submit(Task t) {
    if (!should_stop_.load()) 
      tasks_.put(std::move(t));
  }

  void join() {
    {
      std::unique_lock<std::mutex> lock(mu_);
      should_stop_.store(true);
    }
    queue_notification_.notify_all();

    for (auto &worker : workers_)
       if (worker.joinable())
         worker.join();
    workers_.clear();
  }

private:
  void start_worker_threads(size_t workers) {
    for (size_t i = 0; i < workers; i++) 
      workers_.emplace_back([this]() { worker_routine(); });
  };

  void worker_routine() {
    while (true) {
      Task task;
      {
        std::unique_lock<std::mutex> lock(mu_);
        queue_notification_.wait(
            lock, [this]() { return should_stop_ || !tasks_.empty(); });
        if (should_stop_ && tasks_.empty()) {
          break;
        }
        if (tasks_.empty())
          continue;
        task = tasks_.take();
      }
      task();
    }
  };

private:
  blocking_queue<Task> tasks_; // guarded by mu_
  std::vector<std::thread> workers_;

  std::mutex mu_;
  std::atomic<bool> should_stop_;
  std::condition_variable queue_notification_;
};
} // namespace tp
