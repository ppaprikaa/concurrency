#pragma once
#include "spinlock.hpp"

#include <functional>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::literals::chrono_literals;

class Task {
public:
  std::function<void()> DoJob() {
    return [&, this]() {
      std::lock_guard<Spinlock> guard(lock_);
      for (size_t i = 1; i <= 3; i++) {
        std::this_thread::sleep_for(1s);
        std::cout << "operation " << i << ": " << std::this_thread::get_id()
                  << std::endl;
      }
    };
  }

private:
  Spinlock lock_;
};
