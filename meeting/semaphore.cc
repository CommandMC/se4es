/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           S E M A P H O R E                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Semaphore werden zur Synchronisation von Threads verwendet.               */
/*****************************************************************************/

#include "syscall/guarded_organizer.h"
#include "thread/customer.h"
#include "meeting/semaphore.h"

void Semaphore::p() {
  if (counter > 0) {
    counter--;
  } else {
    Customer *current = (Customer *)Guarded_Organizer::instance().Organizer::active();

    enqueue(current);
    Guarded_Organizer::instance().Organizer::block(*current, *this);
  }
}

void Semaphore::v() {
  Customer *queued = (Customer *)dequeue();

  if (queued != NULL) {
    Guarded_Organizer::instance().Organizer::wakeup(*queued);
  } else {
    counter++;
  }
}

