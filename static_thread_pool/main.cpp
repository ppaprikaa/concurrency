#include <iostream>

#include "static_thread_pool.hpp"

int main(int argc, char *argv[]) {
  tp::static_thread_pool thread_pool{3};
  std::atomic<size_t> n;
  size_t tasks = 1000500;

  for (size_t i = 0; i < tasks; i++)
    thread_pool.submit([&n]() {  n.fetch_add(1); });

  thread_pool.join();

  std::cout << "got: " << n.load() << std::endl;
  std::cout << "expected: " << tasks << std::endl;
  return 0;
}
