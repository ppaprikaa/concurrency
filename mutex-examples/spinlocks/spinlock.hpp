#pragma once
#include "spinwait.hpp"
#include <atomic>

class Spinlock {
public:
  void lock() {
    for (;;) {
      if (!lock_.exchange(true, std::memory_order_acquire))
        return;
      while (lock_.load(std::memory_order_relaxed))
        spinwait_.wait();
    }
  }

  bool try_lock() {
    return !lock_.load(std::memory_order_relaxed) &&
           !lock_.exchange(true, std::memory_order_acquire);
  }

  void unlock() { lock_.store(false, std::memory_order_release); }

private:
  std::atomic<bool> lock_ = {0};
  SpinWait spinwait_;
};
