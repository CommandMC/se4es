#include "standby.h"
#include <msp430.h>
#include "device/plugbox.h"
#include "machine/system.h"
#include "machine/cpu.h"
#include "machine/timer.h"
#include "machine/lcd.h"

StandbyMode StandbyMode::inst;

StandbyMode::StandbyMode() : running(false) {}

/* Licht-Button interrupt?  */
bool StandbyMode::prologue() {
  // nur auf lichtbutton reagieren (passendes Interrupt-Flag)
  if (P2IFG & StandbyMode::LIGHT) {
    // Button entprellen und warten, bis Button losgelassen
    do {
      Timer::delay_us(65000);
      Timer::delay_us(65000);
    } while (P2IN & StandbyMode::LIGHT);

    // Interrupt abschalten, Interrupt-Flag loeschen
    P2IE  &= ~StandbyMode::LIGHT;
    P2IFG &= ~StandbyMode::LIGHT;

    // wenn die anwendungssoftware laeuft (running==true), dann in den bootstrap modus wechseln
    if (running) {
      system_call_bsl(); // fuhert reset durch (kehrt nicht zurueck)
    }
    running = true; // anwendungssoftware starten
  }
  return false; // es darf kein epilogue angefordert werden
}

/**
  * Wird vor der main() ausgefuehrt, d.h. der scheduler timer laeuft noch nicht.
  */
void StandbyMode::activate() {
  /* auf Eingang setzen, Pulldown-Widerstaende einschalten */
  P2OUT &= ~StandbyMode::LIGHT;
  P2REN |=  StandbyMode::LIGHT;
  P2IES |=  StandbyMode::LIGHT; // Interrupt bei high-to-low Flanke
  P2IFG &= ~StandbyMode::LIGHT; // Interrupt-Flag loeschen
  P2IE |= StandbyMode::LIGHT; //  Button Interrupt einschalten
  
  /* Programm soll erst einmal schlafen */
  running = false;

  // Button Interrupt registrieren
  Plugbox::instance().assign(Plugbox::Port2, *this);

  // WDT abschalten
  WDTCTL = WDTPW | WDTHOLD;

  // LCD reset
  LCD::instance().clear();

  // "Sleep" auf dem Display anzeigen
  LCDM12 = 0x3d; //S
  LCDM11 = 0x58; //l
  LCDM10 = 0x79; //e
  LCDM9  = 0x79; //e
  LCDM8  = 0x73; //p

  // "ESS" anzeigen
  LCDM2  = 0x97; // E
  LCDM3  = 0xd3; // S
  LCDM4  = 0xd3; // S

  // Schlafmodus
  while(!running) {
    CPU::halt();
  }
  
  CPU::disable_int();
  LCD::instance().clear();

  P2IFG &= ~StandbyMode::LIGHT; // Interrupt-Flag loeschen
  P2IE |= StandbyMode::LIGHT;   // Button Interrupt einschalten

  // Rueckkehr zu main
}
