#include "buttons.h"
#include <msp430.h> // Define acronyms (digital I/O registers)

Buttons Buttons::buttons; // // Singleton instance

// Your code goes here

void Buttons::enable(uint8_t button_bitmap) {
	button_mask |= button_bitmap;
	P2DIR &= ~button_bitmap; // Port für alle aktivierten Buttons als Eingang konfigurieren
	P2OUT &= ~button_bitmap; // Widerstaende als Pulldown konfigurieren
	P2REN |= button_bitmap; // Widerstaende aktivieren
}

uint8_t Buttons::read() {
	return P2IN & button_mask;
}

bool Buttons::pressed(uint8_t button_bitmap) {
	return read() == button_bitmap;
}
