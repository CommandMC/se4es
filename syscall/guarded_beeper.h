/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   G U A R D E D _ B E E P E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zum Beeper.                                     */
/*****************************************************************************/

#ifndef __Guarded_Beeper_include__
#define __Guarded_Beeper_include__

#include "machine/beeper.h"
#include "meeting/bell.h"

class Guarded_Beeper : public Bell, public Beeper {
private:
  Guarded_Beeper (const Guarded_Beeper &copy); // Verhindere Kopieren
  Guarded_Beeper() {}

  /* Wird indirekt aufgerufen, um den Ton wieder auszuschalten */
  virtual void ring();

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_Beeper& instance() {
    static Guarded_Beeper beeper;// Singleton Instanz
    return beeper;
  }

  /* Parameter: Frequenz in Hertz, Dauer in Millisekunden */
  void beep(unsigned int frequency, unsigned int milliseconds);
};

#endif
