#include "user/examplethread.h"
#include "machine/system.h"
#include "machine/lcd.h"

void ExampleThread::action() {
  while (1) {
    /* Sleep for 100ms to save energy */
    sleep(100);
  
    /* Reset watchdog timer */
    watchdog_reset();

    // Your code goes here!
    // LCD::instance().show_digit(1, 1); // Example for LCD access
  }
  // Warning: The action() method should not return
  // -> infinite loop
}
