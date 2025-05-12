#ifndef WATCH_H
#define WATCH_H

#include "machine/timer.h"

class Watch : public Timer {
public:
  /* Datentyp des Zeit-Tickers - bei Aenderungen unten signed_tick_t anpassen! */
  typedef unsigned int tick_t;

private:
  static Watch watch;

  /* Wird fuer korrektes Wrapping in time_after benoetigt */
  typedef int signed_tick_t;

  Watch(const Watch &copy);

  /* us = Mikrosekunden pro Tick */
  Watch(unsigned long us);

  volatile tick_t system_ticks;

public:
  static Watch& instance() { return watch; }

  /* Uhr starten */
  void windup();

  /* Eigener Timer-Epilog */
  void epilogue();

  /* Aktuelle Systemzeit in Ticks auslesen */
  /* Hinweis: Bei geaendertem Tick-Typ evtl. atomare Operation verwenden! */
  tick_t getticks() { return system_ticks; }
  tick_t now() { return getticks(); }

  /* Rechnet eine Zeit in Millisekunden in eine Anzahl von Ticks um */
  /* (rundet auf)                                                   */
  tick_t ms_to_ticks(unsigned int ms) {
    return (ms * ticktime + 999) / 1000;
  }

  /* Liefert wahr, wenn Tick a nach Tick b liegt */
  static bool time_after(tick_t a, tick_t b) {
    return ((signed_tick_t)b - (signed_tick_t)a) < 0;
  }

  /* Liefert wahr, wenn Tick a vor Tick b liegt */
  static bool time_before(tick_t a, tick_t b) {
    return time_after(b,a);
  }

  /* Liefert wahr, wenn Tick t schon vorbei ist */
  bool after(tick_t t) {
    return time_after(getticks(), t);
  }

  /* Liefert wahr, wenn Tick t noch nicht vorbei ist */
  bool before(tick_t t) {
    return time_before(getticks(), t);
  }

};

extern "C"
void delay_us(unsigned int us);

#endif
