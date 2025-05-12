#ifndef TIMER_H
#define TIMER_H

#include "guard/gate.h"

class Timer : public Gate {
private:
  Timer(const Timer &copy);

protected:
  /* Speichert die Ticks pro Sekunde */
  unsigned long ticktime;

  /* Us = Mikrosekunden pro Tick */
  Timer(unsigned long us);

public:
  /* Klasse in Plugbox anmelden und Timer starten */
  void plugin();

  /* Interner Interrupt-Kram */
  bool prologue();

  /* Wartet ungefaehr die angegebene Zahl von Mikrosekunden, eher mehr */
  static void delay_us(unsigned int us);


  /** Millisekunden-Zaehler **/
private:
  static unsigned int counter;
  static unsigned int overflow;
  static unsigned int to_ms_divisor;

public:
  /* Startet den Millisekunden-Zaehler. Ist dieser bereits gestartet, so wird neugestartet. */
  static void start();
  
  /* Stoppt den Millisekunden-Zaehler. Der gezaehlte Wert bleibt dabei erhalten. */
  static void stop();
  
  /* Millisekunden-Zaehler auslesen. Zuvor muss der Zaehler zwingend gestoppt werden! */
  static unsigned int getcycles() { return (counter + overflow) / to_ms_divisor; }
};

#endif
