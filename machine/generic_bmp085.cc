#include "generic_bmp085.h"

bool GenericBMP085::init() {
  return init_sensor();
}

bool GenericBMP085::set_mode(TPSensor::Mode mode) {
  switch (mode) {
    case TPSensor::Mode::LOW_POWER:
      BMP085::set_mode(BMP085::Mode::ULTRA_LOW_POWER);
      break;
    case TPSensor::Mode::HIGH_RESOLUTION:
      BMP085::set_mode(BMP085::Mode::ULTRA_HIGH_RESOLUTION);
      break;
    default:
      return false;
  }
  return true;
}

int GenericBMP085::get_temperature() {
  return read_temp();
}

long GenericBMP085::get_pressure() {
  return read_pressure();
}
