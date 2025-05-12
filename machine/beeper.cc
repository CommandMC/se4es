#include "config.h"
#include <msp430.h>
#include <in430.h>
#include "machine/beeper.h"

#ifndef BV
#define BV(x) (1U << (x))
#endif

Beeper::Beeper() {
  /* Timer 1 fuer Beeper verwenden, 32768Hz Basistakt, noch nicht zaehlen */
  TA1CTL = TASSEL__ACLK | ID_0 | MC__STOP;
  TA1EX0 = 0;
  TA1CCR0 = 0;

  /* Portmapping umstellen: Timer 1 Capture 1 an Piezo */
  PMAPPWD = PMAPPW;
  PMAPCTL = PMAPRECFG;
  P2MAP7  = PM_TA1CCR1A;

  /* Pin als Ausgang definieren, auf Low setzen, aber noch GPIO bleiben */
  P2DIR |=  BV(7);
  P2OUT &= ~BV(7);
  P2SEL &= ~BV(7);
}

void Beeper::beep(unsigned int frequency) {
  if (frequency != 0) {
    unsigned int tvalue = 32768U / frequency;

    /* Timer 1 starten */
    TA1R = 0;
    TA1CTL   = TASSEL__ACLK | ID_0 | MC__UP | TACLR;
    TA1CCTL1 = OUTMOD_3; //UTMOD_SET_RESET; /* PWM output mode: 3 - PWM set/reset */

    /* Zeitkonstante setzen */
    TA1CCR0 = tvalue;
    TA1CCR1 = tvalue/2;

    /* Piezo-Pin auf Timer legen */
    P2DIR |= BV(7);
    P2SEL |= BV(7);
  }
}

void Beeper::stop() {
  /* Signal abschalten, spart Strom */
  TA1CTL = MC__STOP;
  P2SEL &= ~BV(7);
  P2OUT &= ~BV(7);
}
