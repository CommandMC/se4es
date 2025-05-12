/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                        W A I T I N G R O O M                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Liste von Threads, die auf ein Ereignis warten.                           */
/*****************************************************************************/

#ifndef __waitingroom_include__
#define __waitingroom_include__

#include "object/queue.h"

class Customer; // Forward declaration

class Waitingroom : public Queue {
private:
  Waitingroom(const Waitingroom &copy); // Verhindere Kopieren

protected:
  ~Waitingroom() {} // Verhindere direktes zerstoeren
  void clear(); // Wecke alle wartenden Threads

public:
  Waitingroom() {}

  virtual void remove(Customer *customer);
};

#endif
