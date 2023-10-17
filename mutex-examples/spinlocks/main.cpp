#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Task.hpp"

int main(int argc, char *argv[]) {
  Task task;

  std::thread tr1(task.DoJob());
  std::thread tr2(task.DoJob());
  std::thread tr3(task.DoJob());

  tr1.join();
  tr2.join();
  tr3.join();

  return 0;
}
