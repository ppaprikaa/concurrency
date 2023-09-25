#include <array>
#include <atomic>
#include <iostream>
#include <cstddef>

// Bad mutex, cause:
// 1. thread1 sets it's bool flag to true
// 2. thread2 sets it's bool flag to true
// 3. when they mutually read values of each other flags, they'll wait infinitely, cause both of them are waiting.
// pros: safety is provided 
// cons: liveness is failed
class BrokenMutex1
{
   public:
      BrokenMutex1()
      {
         want_[0].store(false);
         want_[1].store(false);
      }

      void Lock(size_t index)
      {
         want_[index].store(true);
         while(want_[1 - index].load())
         {
            //back off
         }

         owner_ = index;
      }

      void Unlock()
      {
         want_[owner_].store(false);
      }

   private:
      std::array<std::atomic<bool>, 2> want_;
      size_t owner_;
};

// bad cause if there's only one thread, 
// then no one will release first thread from being a victim 
// and it will end up being locked until the end.
// pros: provides safety
// cons: failes liveness
class BrokenMutex2
{
   public:
      void Lock(size_t victimIndex)
      {
         victim_.store(victimIndex);
         while(victimIndex == victim_.load()) 
         {
            // Back off
         }
      }

      void Unlock()
      {
         // Nop
      }
   private:
      std::atomic<size_t> victim_;
};

// if both of threads entered critical area and are taking mutex,
// then latest of them will back off, cause latest will be a victim,
// when non victim will continue working
// and when unlocked second thread can take mutex, 
// cause first one stops taking a mutex.
class PetersonMutex
{
   public:
      PetersonMutex()
      {
         want_[0].store(false);
         want_[1].store(false);
      }

      void Lock(size_t index)
      {
         want_[index].store(true);
         victim_.store(index);

         while(want_[1 - index].load() && victim_.load() == index) {
            // Back off
         }

         owner_ = index;
      }

      void Unlock()
      {
         want_[owner_].store(false);
      }
   private:
      std::array<std::atomic<bool>, 2> want_;
      std::atomic<size_t> victim_;
      size_t owner_;
};