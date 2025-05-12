/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 P A N I C                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Standard Unterbrechungsbehandlung.                                        */
/*****************************************************************************/

#ifndef __panic_include__
#define __panic_include__

/* INCLUDES */

#include "guard/gate.h"

/* Im System verwendete Panic-Nummern */
enum {
  PANIC_PROLOGUE = 8000,   // Panic::prologue aufgerufen
  PANIC_THREADEXIT,        // Thread hat seine action()-Methode verlassen
  PANIC_PUREVIRTUAL,       // "virtual" Methode ohne Implementierung aufgerufen
  PANIC_DOUBLEENTER,       // guard.enter() doppelt aufgerufen
  PANIC_DOUBLERETNE,       // guard.retne() ohne aktiven Guard aufgerufen
  PANIC_LOCKSTARTUP        // guard.enter() oder guard.retne() vor main() aufgerufen
};

class Panic : public Gate
 {
private:
    static Panic _panic;
    Panic (const Panic &copy); // Verhindere Kopieren
    Panic () {}

    static void panic_hook(unsigned int type);
public:
    static Panic& instance() { return _panic; }

    bool prologue();

    /* Meldet Panik mit Nummer "type"                      */
    /* Als int statt enum, um leichter eigene Paniknummern */
    /* verwenden zu koennen.                               */
    void panic(unsigned int type);
};

#endif
