/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K I C K O F F                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Funktion zum Starten einer Koroutine.                                     */
/* Da diese Funktion nicht wirklich aufgerufen, sondern nur durch eine       */
/* geschickte Initialisierung des Stacks der Koroutine angesprungen wird,    */
/* darf sie nie terminieren. Anderenfalls wuerde ein sinnloser Wert als      */
/* Ruecksprungadresse interpretiert werden und der Rechner abstuerzen.       */
/*****************************************************************************/

#include "guard/guard.h"
#include "thread/coroutine.h"
#include "thread/kickoff.h"

void kickoff(Coroutine *object) {
  Guard::instance().leave();
  object->action();
  while (true); // Die kickoff()-Funktion darf nicht zurueckkehren
}
