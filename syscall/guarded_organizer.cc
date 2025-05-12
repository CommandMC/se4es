/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                    G U A R D E D _ O R G A N I Z E R                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zum Organizer.                                  */
/*****************************************************************************/

#include "guard/guard.h"
#include "guard/secure.h"
#include "syscall/guarded_organizer.h"

Guarded_Organizer Guarded_Organizer::organizer;

void Guarded_Organizer::ready(Thread &that) {
  // Damit die main()-Funktion nicht die Basisklassenfunktion
  // Organizer::ready() aufrufen muss, wird hier als Sonderbehandlung
  // dynamisch geprueft, ob der Guard benutzt werden muss.
  bool unguarded = Guard::instance().avail();
  if (unguarded)
    Guard::instance().enter();

  Organizer::ready(that);

  if (unguarded)
    Guard::instance().leave();
}

void Guarded_Organizer::exit() {
  Secure sec;

  Organizer::exit();
}

void Guarded_Organizer::kill(Thread &that) {
  Secure sec;

  Organizer::kill(that);
}

void Guarded_Organizer::resume() {
  Secure sec;

  Organizer::resume();
}
