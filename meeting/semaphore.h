/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           S E M A P H O R E                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Semaphore werden zur Synchronisation von Threads verwendet.               */
/*****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__

#include "meeting/waitingroom.h"
        
class Semaphore : public Waitingroom {
private:
  Semaphore (const Semaphore &copy); // Verhindere Kopieren
  int counter;
protected:
  ~Semaphore() {} // Verhindere direktes zerstoeren
public:
  Semaphore(int c) : counter(c) {}
  void p();
  void v();
  void wait()   { p(); }
  void signal() { v(); }
};

#endif
