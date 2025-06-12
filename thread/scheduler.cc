/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implementierung des Schedulers.                                           */
/*****************************************************************************/

#include <stddef.h>
#include "machine/cpu.h"
#include "device/panic.h"
#include "guard/guard.h"
#include "thread/scheduler.h"
#include "machine/timer.h"


unsigned long Scheduler::idle_time = 0;

void Scheduler::ready(Entrant &proc) {
  if (&proc == active())
    // Derzeit aktiver Thread soll aktivierbar werden: Idlen beenden
    idling = false;
  else
    // Thread in Queue legen
    processqueue.enqueue(&proc);
}

void Scheduler::schedule() {
  // Scheduling-Start verhindern, wenn bereits ein aktiver Thread existiert
  if (active() != NULL)
    return;

  Entrant *e = static_cast<Entrant *>(processqueue.dequeue());

  if (e != NULL)
    go(*e);
}

void Scheduler::exit() {
  // Sich selbst beenden durch Wechsel zum naechsten Thread
  Entrant *e = static_cast<Entrant *>(processqueue.dequeue());

  if (e == NULL) {
    /* Kein Prozess in Readyliste, also idlen */
    e = static_cast<Entrant *>(active());
    idling = true;

    /* anstehende Epiloge abarbeiten, koennten evtl. ready() aufrufen */
    Guard::instance().leave();

    /* Interrupts sperren */
    CPU::disable_int();

    /* Idlen, bis das Flag zurueckgesetzt wurde oder */
    /* ein Folgethread in der Queue steht            */
    int temp_idle_time = 0;
    while (idling) {
      /* Pruefen, ob ein Folgethread existiert */
      e = static_cast<Entrant *>(processqueue.dequeue());
      if (e != NULL)
        break;

      // Zeitmessung starten
      Timer::start();

      /* Idlen - schaltet Interrupts wieder frei */
      CPU::idle();

      // Zeitmessung beenden
      Timer::stop();
      temp_idle_time += Timer::getcycles();

      /* Interrupts wieder sperren - wegen freiem Guard */
      /* wurden alle Epiloge hier schon abgearbeitet    */
      CPU::disable_int();

      if (!idling)
        e = static_cast<Entrant *>(active());
    }
    idle_time = temp_idle_time;

    idling = false;

    /* Guard wieder betreten, Interrupts freigeben */
    Guard::instance().enter();
    CPU::enable_int();
  }

  /* Zum naechsten Thread wechseln */
  dispatch(*e);
}

void Scheduler::kill(Entrant &proc) {
  if (active() == &proc)
    exit();
  else
    processqueue.remove(&proc);
}

void Scheduler::resume() {
  // Nichts machen, wenn das Scheduling noch nicht gestartet wurde
  if (active() == NULL)
    return;

  // Nichts machen, wenn gerade geidlet wird
  if (idling)
    return;

  // Aktuellen Thread in Queue legen
  processqueue.enqueue(static_cast<Entrant*>(active()));

  Entrant *e = static_cast<Entrant *>(processqueue.dequeue());
  // Sollte niemals NULL sein, da gerade etwas in die Queue gelegt wurde

  dispatch(*e);
}

// Gibt Idle-Time zurück.
unsigned long Scheduler::getIdleTime() {
    return idle_time;
}
