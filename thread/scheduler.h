/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implementierung des Schedulers.                                           */
/*****************************************************************************/

#ifndef __schedule_include__
#define __schedule_include__

#include "thread/dispatch.h"
#include "thread/entrant.h"
#include "object/queue.h"

class Scheduler : public Dispatcher
{
private:
  Scheduler (const Scheduler &copy); // Verhindere Kopieren

  Queue processqueue;

  volatile bool idling;

protected:
 Scheduler() : idling(false) {}

public:
  /* Thread beim Scheduler anmelden */
  void ready(Entrant &proc);

  /* Scheduling starten */
  void schedule();

  /* Aktiven Thread beenden */
  void exit();

  /* Anderen Thread beenden */
  void kill(Entrant &proc);

  /* Zum naechsten Thread wechseln */
  void resume();
};

#endif
