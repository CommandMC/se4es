#ifndef BUTTONSERVICE_H
#define BUTTONSERVICE_H

#include <cstdint> // for uint8_t
#include "machine/buttons.h"
#include "syscall/thread.h"

class ButtonService : public UserThread<256> {
public:
  static const uint8_t UP   = Buttons::UP;
  static const uint8_t DOWN = Buttons::DOWN;
  static const uint8_t HASH = Buttons::HASH;
  static const uint8_t STAR = Buttons::STAR;

  typedef uint8_t ButtonState;

private:
  ButtonState button_state; // debounced button state

  ButtonService();
  ButtonService(const ButtonService &copy); // prevent copy

public:
  // Get singleton instance
  static ButtonService& instance()
  {
    static ButtonService bt;
    return bt;
  }

  void action();

  uint8_t get_state() const { return button_state; }

  // Get debounced button state
  ButtonState get_buttons();
};

#endif
