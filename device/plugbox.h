/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              P L U G B O X                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Abstraktion einer Interruptvektortabelle. Damit kann man die Adresse der  */
/* Behandlungsroutine fuer jeden Hardware-, Softwareinterrupt und jede       */
/* Prozessorexception festlegen.                                             */
/*****************************************************************************/

#ifndef __Plugbox_include__
#define __Plugbox_include__

#include "guard/gate.h"

class Plugbox {
public:
  /* Maschinenspezifische Vektorliste hier einbinden */
#  include "machine/plugbox_vectors.h"

private:
  static Plugbox plugbox;

  Plugbox(const Plugbox &copy); // Verhindere Kopieren
  Plugbox();

  Gate *gates[PLUGBOX_VECTOR_COUNT];

public:
  static Plugbox& instance() { return plugbox; }
  void assign(unsigned int slot, Gate &gate);
  Gate &report(unsigned int slot);
};

#endif
