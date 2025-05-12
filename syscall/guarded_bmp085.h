/*****************************************************************************/
/* Software ubiquitaerer Systeme                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       G U A R D E D _ B M P 0 8 5                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum BMP085-Sensor-Objekt             */
/*****************************************************************************/

#ifndef GUARDED_BMP085_H
#define GUARDED_BMP085_H

#include "machine/bmp085.h"
#include "guard/secure.h"

class Guarded_BMP085 : public BMP085 {
private:
  Guarded_BMP085(const Guarded_BMP085 &copy); // Verhindere Kopieren
  Guarded_BMP085() {}

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_BMP085& instance() {
    Secure sec;
    static Guarded_BMP085 bmp085;
    bmp085.init_sensor();
    return bmp085;
  }

  /* Setzt die Auflösung der Druckmessung */
  void set_mode(Mode::mode_t mode) {
    Secure sec;
    BMP085::set_mode(mode);
  }

  /* Misst und gibt die Temperatur in 0.1°C aus */
  int read_temp() {
    Secure sec;
    return BMP085::read_temp();
  }

  /* Misst und gibt den Druck in 1Pa aus */
  long read_pressure() {
    Secure sec;
    return BMP085::read_pressure();
  }
};

#endif
