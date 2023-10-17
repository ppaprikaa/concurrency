#pragma once
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

template <typename T> class blocking_queue {
public:
  void put(T item) {
    std::lock_guard<std::mutex> guard(mu_);
    buffer_.push_back(std::move(item));
    not_empty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mu_);
    while (buffer_.empty())
      not_empty_.wait(lock);

    return take_locked();
  }

  // if it returns correct value for the moment
  // it works correct
  // to ensure correctness for a period of time
  // use synchronization primitives
  bool empty() { return buffer_.empty(); };

private:
  T take_locked() {
    T front = std::move(buffer_.front());
    buffer_.pop_front();
    return front;
  }

private:
  std::deque<T> buffer_; // Guarded by mu_
  std::mutex mu_;
  std::condition_variable not_empty_;
};
