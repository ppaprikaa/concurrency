#include <iostream>
#include <fmt/core.h>
#include <thread>
#include <unistd.h>

#include "two_threads.hpp"

PetersonMutex mu;
static int i = 0;

void doWork()
{
   int k = i++;
   mu.Lock(k);
   fmt::println("Hello {}", k);
   fmt::println("How are you?  {}", k);
   mu.Unlock();
}

int main(int argc, char * argv[])
{
   std::thread worker1(doWork);
   std::thread worker2(doWork);
   worker1.join();
   worker2.join();
   fmt::println("Please press 'Enter' to exit.");
   std::cin.get();
   return 0;
}