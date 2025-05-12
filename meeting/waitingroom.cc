/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                        W A I T I N G R O O M                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Liste von Threads, die auf ein Ereignis warten.                           */
/*****************************************************************************/

#include "syscall/guarded_organizer.h"
#include "meeting/waitingroom.h"

void Waitingroom::clear() {
  Customer *cust;

  while ((cust = (Customer *)dequeue())) {
    Guarded_Organizer::instance().Organizer::wakeup(*cust);
  }
}

void Waitingroom::remove(Customer *customer) {
  Queue::remove(customer);
}
