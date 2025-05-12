/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          D I S P A T C H E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implementierung des Dispatcher.                                           */
/* Der Dispatcher verwaltet den life-Pointer, der die jeweils aktive         */
/* Koroutine angibt. mit go() wird der life Pointer initialisiert und die    */
/* erste Koroutine gestartet, alle weiteren Kontextwechsel werden mit        */
/* dispatch() ausgeloest. active() liefert den life Pointer zurueck.         */
/*****************************************************************************/

#ifndef __dispatch_include__
#define __dispatch_include__

#include <stddef.h>
#include "thread/coroutine.h"
        
class Dispatcher {
private:
  Dispatcher(const Dispatcher &copy); // Verhindere Kopieren

  Coroutine *life;

protected:
  Dispatcher() : life(NULL) { }

public:
  void go(Coroutine &first);
  void dispatch(Coroutine &next);
  Coroutine *active() { return life; }
};

#endif
