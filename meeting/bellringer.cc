/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          B E L L R I N G E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Der "Gloeckner" wird im Rahmen der Timer-Epilog Bearbeitung aktiviert.    */
/* Er prueft, ob irgendwelche Glocken (Bell-Objekte) laeuten muessen. Die    */
/* Glocken befinden sich in einer Queue, die der Gloeckner verwaltet.        */
/*****************************************************************************/

#include <stddef.h>
#include "meeting/bellringer.h"

Bellringer Bellringer::bellringer;

void Bellringer::check() {
  Bell *b = static_cast<Bell *>(first());
  
  if (b == NULL)
    return;

  b->tick();
  while (b && b->run_down()) {
    dequeue();
    b->ring();
    b = static_cast<Bell *>(first());
  }
}

void Bellringer::job(Bell *bell, unsigned int ticks) {
  Bell *b;

  bell->wait(ticks);

  b = static_cast<Bell *>(first());
  if (!b) {
    // Empty list
    enqueue(bell);
  } else {
    if (ticks < b->wait()) {
      // Head item of list has a longer wait time
      b->wait(b->wait() - ticks);
      insert_first(bell);
    } else {
      // Find correct position in list to enqueue
      Bell *prevb = b;

      while (b && ticks >= b->wait()) {
        ticks -= b->wait();
        prevb = b;
        b = static_cast<Bell *>(b->next);
      }

      if (b != NULL)
        // Adjust delta-ticks of next item
        b->wait(b->wait() - ticks);

      bell->wait(ticks);
      insert_after(prevb, bell);
    }
  }
}

void Bellringer::cancel(Bell *bell) {
  Bell *b = static_cast<Bell *>(first());

  // Search bell in list
  while (b && b != bell)
    b = static_cast<Bell *>(b->next);

  if ((b == bell) && (b != NULL)) {
    b = static_cast<Bell *>(b->next);
    if (b != NULL)
      // Adjust delta-ticks of next item
      b->wait(b->wait() + bell->wait());
    remove(bell);
  }
}

