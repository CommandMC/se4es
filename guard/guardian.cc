/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              G U A R D I A N                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Zentrale Unterbrechungsbehandlungsroutine des Systems.                    */
/* Der Parameter gibt die Nummer des aufgetretenen Interrupts an.            */
/*****************************************************************************/

/* INCLUDES */

#include "device/panic.h"
#include "device/plugbox.h"
#include "guard/guard.h"

/* FUNKTIONEN */
               
extern "C" void guardian (unsigned int slot);

void guardian (unsigned int slot) {
  Gate &handler = Plugbox::instance().report(slot);
  if (&handler==&Panic::instance())
	  Panic::instance().panic(slot);

  if (handler.prologue())
    Guard::instance().relay(&handler);
}
