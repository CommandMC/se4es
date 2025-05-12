/*****************************************************************************/
/* ESC                                                                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                     G U A R D E D _ T P _ S E N S O R                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Interface for application threads to the temperature and pressure sensor  */
/*****************************************************************************/

#ifndef GUARDED_TP_SENSOR_H
#define GUARDED_TP_SENSOR_H

#include "machine/tp_sensor.h"
#include "guard/secure.h"

class Guarded_TPSensor : public TPSensor {
  TPSensor &sensor;

  Guarded_TPSensor(const Guarded_TPSensor &copy) = default; // Prevent copies in client code
  Guarded_TPSensor(TPSensor &s) : sensor(s) {}

public:
  /* Access to the singleton instance */
  static Guarded_TPSensor &instance() {
    Secure sec;
    static Guarded_TPSensor tps = TPSensor::instance();
    return tps;
  }

  bool init() override {
    Secure sec;
    return sensor.init();
  }

  /* Set operating mode */
  bool set_mode(Mode mode) override {
    Secure sec;
    return sensor.set_mode(mode);
  }

  /* Get temperature from sensor in 0.1 degree Celsius.
   * For a temperature of 20.5 degree Celsius this method returns 205.
   */
  int get_temperature() override {
    Secure sec;
    return sensor.get_temperature();
  }

  /* Get pressure in Pascal */
  long get_pressure() override {
    Secure sec;
    return sensor.get_pressure();
  }
};

#endif
