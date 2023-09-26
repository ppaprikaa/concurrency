#pragma once
#include <atomic>

// Test-and-Set (TAS) Spinlock
class TASSpinlock
{
   public:
      void Lock()
      {
         while(locked_.exchange(true))
         {
            // Back off
         }
      }

      void Unlock()
      {
         locked_.store(false);
      }

   private:
      std::atomic<bool> locked_;
};