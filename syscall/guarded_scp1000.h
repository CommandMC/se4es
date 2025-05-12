/*****************************************************************************/
/* Software ubiquitaerer Systeme                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       G U A R D E D _ S C P 1 0 0 0                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum SCP1000-Sensor-Objekt             */
/*****************************************************************************/

#ifndef GUARDED_SCP1000_H
#define GUARDED_SCP1000_H

#include "machine/scp1000.h"
#include "guard/secure.h"

class Guarded_SCP1000 : public SCP1000 {
private:
  Guarded_SCP1000(const Guarded_SCP1000 &copy); // Verhindere Kopieren
  Guarded_SCP1000() {}

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_SCP1000& instance() {
    Secure sec;
    static Guarded_SCP1000 scp1000;
    scp1000.init_sensor();
    return scp1000;
  }

  /* Setzt den Auswertemodus */
  void set_sensor_mode(SensorMode mode) {
    Secure sec;
    SCP1000::set_sensor_mode(mode);
  }

  /* Liest den zuletzt gemessenen Temperaturwert aus */
  int read_temp() {
    Secure sec;
    return SCP1000::read_temp();
  }

  /* Liest den zuletzt gemessenen Druckwert aus */
  long read_pressure() {
    Secure sec;
    return SCP1000::read_pressure();
  }
};

#endif
