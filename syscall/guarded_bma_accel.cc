/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                        G U A R D E D _ A C C E L                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum Accel-Objekt                      */
/*****************************************************************************/

#include "syscall/guarded_bma_accel.h"
#include "guard/secure.h"

Guarded_BMA_Accel::Guarded_BMA_Accel() {
  plugin();
}

void Guarded_BMA_Accel::set_range(Range range) {
  Secure sec;
  BMA250::set_range(range);
}

void Guarded_BMA_Accel::set_bandwidth(Bandwidth bw) {
  Secure sec;
  BMA250::set_bandwidth(bw);
}

void Guarded_BMA_Accel::enable_normal_mode() {
  Secure sec;
  BMA250::enable_normal_mode();
}

void Guarded_BMA_Accel::enable_lowpower_mode(SleepPhase sleep) {
  Secure sec;
  BMA250::enable_lowpower_mode(sleep);
}

/* Generic sensor interface ----------------------------------------------- */
void Guarded_BMA_Accel::enable(AccelSensor::Range range, SampleRate rate) {
  Secure sec;
  BMA250::enable(range, rate);
}

void Guarded_BMA_Accel::suspend() {
  Secure sec;
  BMA250::suspend();
}

bool Guarded_BMA_Accel::data_available() {
  Secure sec;
  return BMA250::data_available();
}

signed char Guarded_BMA_Accel::read_x() {
  Secure sec;
  return BMA250::read_x();
}

signed char Guarded_BMA_Accel::read_y() {
  Secure sec;
  return BMA250::read_y();
}

signed char Guarded_BMA_Accel::read_z() {
  Secure sec;
  return BMA250::read_z();
}
