#include "config.h"
#include <msp430.h>
#include "machine/timer.h"

Timer::Timer(unsigned long us) {
  ticktime = 1000000L/us;
}

void Timer::plugin() {
  /** Timer 0: Systemtimer **/
#if 1
  // Stromsparende Variante
  /* Kein weiterer Teiler */
  TA0EX0 = 0;

  /* Maximalen Zaehlerwert einstellen */
  TA0CCR0 = (32878 / ticktime) - 1;

  /* Timer 0 zaehlt ACLK aufwaerts */
  TA0CTL = TASSEL__ACLK | ID_0 | MC__UP;

  to_ms_divisor = 32;
#endif

#if 0
  // Exakte Variante
  /* Timer 0 zaehlt SMCLK/8 aufwaerts */
  /* Weiteren /4-Teiler einfuegen */
  TA0EX0 = (4-1);

  /* Maximalen Zaehlerwert einstellen */
  TA0CCR0 = (CONFIG_CPU_FREQUENCY / 8 / 4 / ticktime) - 1;

  /* Timer 0 zaehlt SMCLK/8 aufwaerts */
  TA0CTL = TASSEL__SMCLK | ID_3 | MC__UP;

  to_ms_divisor = 1; // TODO: Wert berechnen
#endif

#if 0
  // Cycles Variante
  // Keinen Teiler einfuegen
  TA0EX0 = 0;

  // Maximalen Zaehlerwert einstellen
  TA0CCR0 = 0xFFFF;

  //Timer 0 zaehlt SMCLK aufwaerts
  TA0CTL = TASSEL__SMCLK | ID_0 | MC__UP;

  to_ms_divisor = 1;
#endif

  /* Timer-Interrupts zulassen */
  TA0CTL |= TAIE;
}

bool Timer::prologue() {
  if (TA0IV == 0x0e) {
    /* Timer overflow */
    overflow += TA0CCR0;
    return true;
  } else
    /* Andere Timer-Interrupts ignorieren */
    return false;
}

void Timer::delay_us(unsigned int us) {
  /* Grob, aber ausreichend */
  for (unsigned int i=0; i<us/5; i++)
    asm volatile ("nop");
}

/* Wird fuer TI-Simplicity benoetigt */
extern "C" void delay_us(unsigned int us) {
  Timer::delay_us(us);
}

/** Millisekunden-Zaehler **/
unsigned int Timer::counter;
unsigned int Timer::overflow;
unsigned int Timer::to_ms_divisor;

void Timer::start() {
  overflow = 0;
  counter = TA0R;
}

void Timer::stop() {
  const unsigned int now = TA0R;
  counter = now - counter;
}

