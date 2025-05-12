#include "generic_scp1000.h"

bool GenericSCP1000::init() {
  return init_sensor();
}

bool GenericSCP1000::set_mode(TPSensor::Mode mode) {
  switch (mode) {
    case Mode::LOW_POWER:
      set_trigger_mode(SCP1000::TriggerMode::LOW_RESOLUTION);
      break;
    case Mode::HIGH_RESOLUTION:
      set_trigger_mode(SCP1000::TriggerMode::HIGH_RESOLUTION);
      break;
    default:
      return false;
  }
  return true;
}

int GenericSCP1000::get_temperature() {
  trigger_measurement();
  return read_temp()/2;
}

long GenericSCP1000::get_pressure() {
  trigger_measurement();
  return read_pressure()/4;
}
