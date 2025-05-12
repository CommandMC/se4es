#ifndef BUTTONS_H
#define BUTTONS_H

#include <inttypes.h> // for uint8_t
#include <msp430.h>

// Here you have to add your code

class Buttons {
private:
  static Buttons buttons; // Singleton instance

  uint8_t button_mask;

  Buttons(const Buttons &copy); // Prevent copies
  Buttons() : button_mask(0) {}

public:
  /* Get access to the singleton instance */
  static Buttons& instance() {
    return buttons;
  }

  /* 
   * The following constants define the bits associated with
   * the buttons on the Chronos watch. You have to find out which bit is
   * assigned to which button in the "eZ430-Chronos User's Guide (slau292)".
   * You can use the constants BIT0, BIT1, ..., BIT7 from
   * the system header msp430.h (after including it).
   */
  static const uint8_t UP   = BIT4; // P2.4
  static const uint8_t DOWN = BIT0; // P2.0
  static const uint8_t HASH = BIT1; // P2.1
  static const uint8_t STAR = BIT2; // P2.2

  /*
   * enable(btn): activate the specified button(s)
   */
  void enable(uint8_t button_bitmap);

  /*
   * read(): get the current state of all buttons
   */
  uint8_t read();

  /*
   *  pressed(btn): return true, if the specified button(s) is(are)
   *                currently pressed.
   *                Example: pressed(Buttons::UP | Buttons::DOWN);
   *                -> returns true, if UP and DOWN are pressed
   */
  bool pressed(uint8_t button_bitmap);
};

#endif /* BUTTONS_H */
