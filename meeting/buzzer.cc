/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                               B U Z Z E R                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Mit Hilfe des "Weckers" koennen Prozesse eine bestimmte Zeit lang         */
/* schlafen und sich dann wecken lassen.                                     */
/*****************************************************************************/

#include "device/watch.h"
#include "meeting/buzzer.h"
#include "meeting/bellringer.h"
#include "syscall/guarded_organizer.h"

void Buzzer::clear() {
  Bellringer::instance().cancel(this);
  Waitingroom::clear();
}

void Buzzer::ring() {
  Customer *c;

  while ((c = static_cast<Customer *>(dequeue()))) {
    Guarded_Organizer::instance().Organizer::wakeup(*c);
  }
}

void Buzzer::set(unsigned int ms) {
  buzzticks = Watch::instance().ms_to_ticks(ms);
}

void Buzzer::sleep() {
  Customer *me = (Customer *)Guarded_Organizer::instance().Organizer::active();

  enqueue(me);
  Bellringer::instance().job(this, buzzticks);
  Guarded_Organizer::instance().Organizer::block(*me, *this);
}

void Buzzer::remove(Customer *target) {
  Waitingroom::remove(target);
  if (head == NULL)
    Bellringer::instance().cancel(this);
}
