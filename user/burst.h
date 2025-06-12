#ifndef BURST_H
#define BURST_H

#include "syscall/thread.h"

class BurstService : public UserThread<256> {
public:
  [[noreturn]] void action() override;

  static BurstService& instance() {
    static BurstService burst;
    return burst;
  }
};

#endif