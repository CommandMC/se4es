/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          O R G A N I Z E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Ein Organizer ist ein spezieller Scheduler, der zusaetzlich das Warten    */
/* von Threads (Customer) auf Ereignisse erlaubt.                            */
/*****************************************************************************/

#include <stddef.h>
#include "device/panic.h"
#include "thread/customer.h"
#include "thread/organizer.h"

void Organizer::block(Customer &customer, Waitingroom &waitingroom) {
  customer.waiting_in(&waitingroom);
  exit();
}

void Organizer::wakeup(Customer &customer) {
  customer.waiting_in(NULL);
  ready(customer);
}

void Organizer::kill(Customer &target) {
  if (target.waiting_in() != NULL) {
    Waitingroom *w = target.waiting_in();

    w->remove(&target);
    target.waiting_in(NULL);
  } else {
    Scheduler::kill(target);
  }
}

