/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                     G U A R D E D _ B M A _ A C C E L                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum Accel-Objekt                      */
/*****************************************************************************/

#ifndef GUARDED_BMA_ACCEL
#define GUARDED_BMA_ACCEL

#include "machine/bma250.h"
#include "guard/secure.h"

class Guarded_BMA_Accel : public BMA250 {
private:
  Guarded_BMA_Accel(const Guarded_BMA_Accel &copy); // Verhindere Kopieren
  Guarded_BMA_Accel();

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_BMA_Accel &instance() {
    Secure sec;
    static Guarded_BMA_Accel accel; // Singleton Instanz
    return accel;
  }

  void set_range(Range range);
  void set_bandwidth(Bandwidth bw);

  void enable_normal_mode();
  void enable_lowpower_mode(SleepPhase sleep);

  /* Generic sensor interface ----------------------------------------------- */
  void enable(AccelSensor::Range range, SampleRate rate) override;
  void suspend() override;

  bool data_available() override;

  signed char read_x() override;
  signed char read_y() override;
  signed char read_z() override;
};

#endif
