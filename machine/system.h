#ifndef SYSTEM_H
#define SYSTEM_H

#include <msp430.h>

/* Watchdog zuruecksetzen */
static inline void watchdog_reset(void) { WDTCTL = WDTPW | WDTCNTCL; }


#ifdef __cplusplus
extern "C" {
#endif

/*
 * VCore and Takte konfigurieren, Watchdog einschalten.
 */
void system_init(void);

void clocks_init(long cfreq);

/*
 * VCore and Takte in den Ausgangszustand versetzen, Watchdog ausschalten.
 * (Gegenbenfalls eingeschaltete) Peripherie abschalten.
 */
void system_restore(void);

void system_call_bsl(void);

#ifdef __cplusplus
}
#endif

#endif
