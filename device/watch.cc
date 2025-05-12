#include "device/watch.h"
#include "device/plugbox.h"
#include "meeting/bellringer.h"
#include "syscall/guarded_organizer.h"

Watch Watch::watch(10000);

Watch::Watch(unsigned long us) : Timer(us), system_ticks(0) {}

void Watch::windup() {
  /* Interrupthandler anmelden */
  // FIXME: Umstellen auf CC0 statt Misc!
  Plugbox::instance().assign(Plugbox::Timer0_Misc, *this);

  Timer::plugin();
}

void Watch::epilogue() {
  system_ticks++;
  Bellringer::instance().check();
  Guarded_Organizer::instance().Organizer::resume();
}
