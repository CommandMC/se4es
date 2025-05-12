/*****************************************************************************/
/* Software ubiquitaerer Systeme                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       G U A R D E D _ R T C                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum RTC-Objekt                        */
/*****************************************************************************/

#ifndef GUARDED_RTC_H
#define GUARDED_RTC_H

#include "machine/rtc.h"
#include "guard/secure.h"

class Guarded_RTC : public RTC {
private:
  static Guarded_RTC rtc; // Singleton Instanz

  Guarded_RTC(const Guarded_RTC &copy); // Verhindere Kopieren
  Guarded_RTC() {}

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_RTC& instance() { return rtc; }

  void set(time_t time) {
    Secure sec;

    RTC::set(time);
  }

  void read(time_t &time) {
    Secure sec;

    RTC::read(time);
  }
};

#endif
