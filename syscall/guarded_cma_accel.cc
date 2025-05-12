/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                    G U A R D E D _ C M A _ A C C E L                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum CMA3000-Objekt                    */
/*****************************************************************************/

#include "syscall/guarded_cma_accel.h"
#include "guard/secure.h"

Guarded_CMA_Accel::Guarded_CMA_Accel() {
  plugin();
}

void Guarded_CMA_Accel::set_mode_range(Mode mode, Range range) {
  Secure sec;
  CMA3000::set_mode_range(mode, range);
}

void Guarded_CMA_Accel::enable(AccelSensor::Range range, SampleRate rate) {
  Secure sec;
  CMA3000::enable(range, rate);
}

void Guarded_CMA_Accel::suspend() {
  Secure sec;
  CMA3000::suspend();
}

bool Guarded_CMA_Accel::data_available() {
  Secure sec;
  return CMA3000::data_available();
}

signed char Guarded_CMA_Accel::read_x() {
  Secure sec;
  return CMA3000::read_x();
}

signed char Guarded_CMA_Accel::read_y() {
  Secure sec;
  return CMA3000::read_y();
}

signed char Guarded_CMA_Accel::read_z() {
  Secure sec;
  return CMA3000::read_z();
}
