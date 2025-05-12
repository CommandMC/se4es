#ifndef CMA3000_H
#define CMA3000_H

// CMA3000 Acceleration Sensor

#include "guard/gate.h"
#include "machine/accel_sensor.h"

class CMA3000 : public Gate, public AccelSensor {
private:
  CMA3000(const CMA3000 &copy);

public:
  CMA3000();

  /* Sensor specific interface ---------------------------------------------- */
  enum class Mode : unsigned char {
    POWERDOWN      = (0<<1),
    MEASURE_100HZ  = (1<<1),
    MEASURE_400HZ  = (2<<1),
    MEASURE_40HZ   = (3<<1),
    MOTION_DETECT  = (4<<1),
    FREEFALL_100HZ = (5<<1),
    FREEFALL_400HZ = (6<<1)
  };

  enum class Range : unsigned char {
    RANGE_8G = 0,
    RANGE_2G = (1<<7)
  };

  // Set sensor specific mode and range
  void set_mode_range(Mode mode, CMA3000::Range range);

  /* Generic sensor interface ----------------------------------------------- */
  void enable(AccelSensor::Range range, SampleRate rate) override;

  bool data_available() override;
  void suspend() override;

  signed char read_x() override;
  signed char read_y() override;
  signed char read_z() override;

  /* Low-level register access ---------------------------------------------- */
  enum class Register : unsigned char {
    WHO_AM_I = 0x00,
    REVID = 0x01,
    CTRL = 0x02,
    STATUS = 0x03,
    RSTR = 0x04,
    INT_STATUS = 0x05,
    DOUTX = 0x06,
    DOUTY = 0x07,
    DOUTZ = 0x08,
    MDTHR = 0x09,
    MDFFTMR = 0x0a,
    FFTHR = 0x0b,
    I2C_ADDR = 0x0c
  };

  unsigned char read_register(Register reg);
  unsigned char write_register(Register reg, unsigned char data);

  /* Driver system interface ------------------------------------------------ */
  void plugin();

  // Gate methods
  bool prologue() override;
};

#endif
