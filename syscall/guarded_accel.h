/*****************************************************************************/
/* ESS-K                                                                     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         G U A R D E D _ A C C E L                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Interface to the Accel-Object for application threads                     */
/*****************************************************************************/

#ifndef GUARDED_ACCEL
#define GUARDED_ACCEL

#include "machine/accel_sensor.h"
#include "guard/secure.h"

class Guarded_Accel : public AccelSensor {
  AccelSensor &sensor;
private:
  Guarded_Accel(const Guarded_Accel &copy); // Prevent copies
  Guarded_Accel(AccelSensor &s) : sensor(s) {}

public:
  /** Get an initialized instance of the sensor */
  static Guarded_Accel& instance() {
    Secure sec;
    static Guarded_Accel accel = AccelSensor::instance(); // Singleton Instanz
    return accel;
  }

  /* Generic sensor interface ----------------------------------------------- */
  /** Start a measurement with the specified range and sample rate */
  void enable(AccelSensor::Range range, SampleRate rate) override {
    Secure sec;
    sensor.enable(range, rate);
  }

  /** Stop the measurement */
  void suspend() override {
    Secure sec;
    sensor.suspend();
  }

  /** Check if acceleration data can be fetched using read_x/y/z */
  bool data_available() override {
    Secure sec;
    return sensor.data_available();
  }

  /** Fetch acceleration value for the x-axis */
  signed char read_x() override {
      Secure sec;
      return sensor.read_x();
  }

  /** Fetch acceleration value for the y-axis */
  signed char read_y() override {
      Secure sec;
      return sensor.read_y();
  }

  /** Fetch acceleration value for the z-axis */
  signed char read_z() override {
      Secure sec;
      return sensor.read_z();
  }
};

#endif
