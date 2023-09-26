#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "ticket_spinlock.hpp"

TicketLock mu;

void DoWork()
{
   using namespace std::literals::chrono_literals;
   mu.Lock();
   std::vector<std::string> messages;
   messages.push_back("operation 1: ");
   messages.push_back("operation 2: ");
   messages.push_back("operation 3: ");

   for (auto msg : messages)
   {
      std::this_thread::sleep_for(1s);
      std::cout << msg << std::this_thread::get_id() << std::endl;
   }
   mu.Unlock();
}

int main(int argc, char * argv[])
{
   std::thread tr1(DoWork);
   std::thread tr2(DoWork);
   std::thread tr3(DoWork);

   tr1.join();
   tr2.join();
   tr3.join();

   return 0;
}