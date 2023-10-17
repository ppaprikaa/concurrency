#pragma once
#include <atomic>
#include <cstdlib>

class TicketLock {
  using Ticket = size_t;

public:
  void Lock() {
    Ticket my_ticket = next_free_ticket_.fetch_add(1);
    while (owner_ticket_.load() != my_ticket) {
      // Back off
    }
  }

  void Unlock() { owner_ticket_.fetch_add(1); }

  bool TryLock() { return false; }

private:
  std::atomic<Ticket> next_free_ticket_;
  std::atomic<Ticket> owner_ticket_;
};
