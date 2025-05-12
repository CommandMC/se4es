/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          B E L L R I N G E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Der "Gloeckner" wird im Rahmen der Timer-Epilog Bearbeitung aktiviert.    */
/* Er prueft, ob irgendwelche Glocken (Bell-Objekte) laeuten muessen. Die    */
/* Glocken befinden sich in einer Liste, die der Gloeckner verwaltet.        */
/*****************************************************************************/

#ifndef __Bellringer_include__
#define __Bellringer_include__

#include "meeting/bell.h"
#include "object/list.h"

class Bellringer : public List {
private:
  static Bellringer bellringer;
  Bellringer(const Bellringer &copy); // Verhindere Kopieren

public:
  Bellringer() {}
  static Bellringer& instance() { return bellringer; }
  
  void check();
  void job(Bell *bell, unsigned int ticks);
  void cancel(Bell *bell);
};

#endif
