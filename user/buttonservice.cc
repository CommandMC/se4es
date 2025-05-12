#include "user/buttonservice.h"
#include "machine/buttons.h"

ButtonService::ButtonService() : button_state(0) {
  Buttons::instance().enable(Buttons::UP | Buttons::DOWN | Buttons::HASH | Buttons::STAR);
}

void ButtonService::action() {
  Buttons& buttons = Buttons::instance();
  static const int debounce_time = 30; // ms
  while (1) {
    uint8_t current = buttons.read();
    if (current != 0) {
      // some button pressed
      button_state = current; // publish new button state
      for (int i=0; i < (500 / debounce_time); i++) {
        // accept the same button press again if 500 ms have elapsed
        sleep(debounce_time); // debounce
        if (buttons.read() != current)
          break; // wait until state change
      }
    }
    else {
      sleep(10); // sample as fast as possible (10 ms)
    }
  }
}

uint8_t ButtonService::get_buttons() {
  uint8_t current = button_state;
  if (current != 0) { // some button pressed?
    button_state = 0; // consume the actual button press, but only if pressed
  }
  return current;
}
