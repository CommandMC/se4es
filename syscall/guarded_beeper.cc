/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   G U A R D E D _ B E E P E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zum Beeper.                                     */
/*****************************************************************************/

/* INCLUDES */

#include "syscall/guarded_beeper.h"
#include "guard/secure.h"
#include "device/watch.h"
#include "meeting/bellringer.h"

/* Parameter: Frequenz in Hertz, Dauer in Millisekunden */
void Guarded_Beeper::beep(unsigned int frequency, unsigned int milliseconds) {
  Secure sec;

  if (run_down()) {
    Beeper::beep(frequency);

    /* Nach dem Ablauf von 'milliseconds' soll ring() aufgerufen werden */
    Bellringer::instance().job(this, Watch::instance().ms_to_ticks(milliseconds));
  }
}

/* Wird indirekt von Bellringer aufgerufen, um den Ton wieder auszuschalten */
void Guarded_Beeper::ring() {
  Beeper::stop();
}
