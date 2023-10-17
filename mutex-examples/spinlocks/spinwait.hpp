#pragma once

#include <iostream>
class SpinWait {
public:
  void wait() {
#if defined(__x86_64__) || defined(_M_X64)
       __builtin_ia32_pause();
#endif
  };
};
