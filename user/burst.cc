#include "burst.h"

[[noreturn]] void BurstService::action() {
  while (true) {
    int my_int = 0;
    for (long i = 0; i < 1000; ++i) {
      my_int += i;
    }

    sleep(1000);
  }
}
