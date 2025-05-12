/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                     G U A R D E D _ C M A _ A C C E L                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum Accel-Objekt                      */
/*****************************************************************************/

#ifndef GUARDED_CMA_ACCEL
#define GUARDED_CMA_ACCEL

#include "machine/cma3000.h"
#include "guard/secure.h"

class Guarded_CMA_Accel : public CMA3000 {
private:
  Guarded_CMA_Accel(const Guarded_CMA_Accel &copy); // Verhindere Kopieren
  Guarded_CMA_Accel();

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_CMA_Accel& instance() {
    Secure sec;
    static Guarded_CMA_Accel accel; // Singleton Instanz
    return accel;
  }

  void set_mode_range(Mode mode, CMA3000::Range range);

  /* Generic sensor interface ----------------------------------------------- */
  void enable(AccelSensor::Range range, SampleRate rate) override;
  void suspend() override;

  bool data_available() override;

  signed char read_x() override;
  signed char read_y() override;
  signed char read_z() override;
};

#endif
