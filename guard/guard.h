/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                G U A R D                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Mit Hilfe dieser Klasse koennen Aktivitaeten, die einen kritischen        */
/* Abschnitt betreffen, der mit einem Guard-Objekt geschuetzt ist, mit       */
/* Unterbrechungsbehandlungsroutinen synchronisiert werden, die ebenfalls    */
/* auf den kritischen Abschnitt zugreifen.                                   */
/*****************************************************************************/

#ifndef __Guard_include__
#define __Guard_include__

#include "guard/locker.h"
#include "guard/gate.h"       
#include "object/queue.h"

class Guard : public Locker
 {
private:
   Queue epilogqueue;
   static Guard guard; // Singleton Instanz

   Guard (const Guard &copy); // Verhindere Kopieren
   Guard() {}

public:
   static Guard& instance() { return guard; }

   void leave();
   void relay(Gate* item);
 };

#endif
