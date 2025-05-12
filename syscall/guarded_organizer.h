/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   G U A R D E D _ O R G A N I Z E R                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zum Organizer.                                  */
/*****************************************************************************/

#ifndef __Guarded_Organizer_include__
#define __Guarded_Organizer_include__

#include "syscall/thread.h"
#include "thread/organizer.h"
        
class Guarded_Organizer : public Organizer
{
private:
  static Guarded_Organizer organizer; // Singleton Instanz

  Guarded_Organizer (const Guarded_Organizer &copy); // Verhindere Kopieren
  Guarded_Organizer () {}

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_Organizer& instance() { return organizer; }

  void ready(Thread &that);
  void exit();
  void kill(Thread &that);
  void resume();
};

#endif
