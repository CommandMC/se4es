/* BMA250 Beschleunigungssensor */
#ifndef BMA250_H
#define BMA250_H

#include "guard/gate.h"
#include "machine/accel_sensor.h"

#define BMA_RSTKEY           (0xB6)

class BMA250 : public Gate, public AccelSensor {
private:
  BMA250(const BMA250 &copy);

protected:
public:
  BMA250();

public:
  /* Sensor specific interface ---------------------------------------------- */
  enum class Range : unsigned char {
    RANGE_2 = 0x03,
    RANGE_4 = 0x05,
    RANGE_8 = 0x08,
    RANGE_16 = 0x0C
  };

  enum class Bandwidth : unsigned char {
    BANDWIDTH_8 = 0x08,
    BANDWIDTH_16 = 0x09,
    BANDWIDTH_31 = 0x0A,
    BANDWIDTH_63 = 0x0B,
    BANDWIDTH_125 = 0x0C,
    BANDWIDTH_250 = 0x0D,
    BANDWIDTH_500 = 0x0E,
    BANDWIDTH_1000 = 0x0F
  };

  enum class SleepPhase : unsigned char { // Low power mode bit included
    SLEEPPHASE_1 = 0x4C,
    SLEEPPHASE_2 = 0x4E,
    SLEEPPHASE_4 = 0x50,
    SLEEPPHASE_6 = 0x52,
    SLEEPPHASE_10 = 0x54,
    SLEEPPHASE_25 = 0x56,
    SLEEPPHASE_50 = 0x58
  };


  /* Setzt den Auswertemodus und die Empfindlichkeit des Sensors */
  void set_range(Range range);
  void set_bandwidth(Bandwidth bw);

  void enable_normal_mode();
  void enable_lowpower_mode(SleepPhase sleep);

  /* Generic sensor interface ----------------------------------------------- */
  void enable(AccelSensor::Range range, AccelSensor::SampleRate rate) override;

  bool data_available() override;
  void suspend() override;

  /* Liest die Beschleunigung in X/Y/Z-Richtung */
  signed char read_x() override;
  signed char read_y() override;
  signed char read_z() override;

  /* Low-level register access ---------------------------------------------- */
  enum class Register : unsigned char {
    CHIPID    = 0x00,
    ACC_X_LSB = 0x02,
    ACC_X_MSB = 0x03,
    ACC_Y_LSB = 0x04,
    ACC_Y_MSB = 0x05,
    ACC_Z_LSB = 0x06,
    ACC_Z_MSB = 0x07,
    GRANGE    = 0x0F, // g Range
    BWD       = 0x10, // Bandwidth
    PM        = 0x11, // Power modes
    SCR       = 0x13, // Special Control Register
    RESET     = 0x14, // Soft reset register (writing 0xB6 causes reset)
    ISR1      = 0x16, // Interrupt settings register 1
    ISR2      = 0x17, // Interrupt settings register 2
    IMR1      = 0x19, // Interrupt mapping register 1
    IMR2      = 0x1A, // Interrupt mapping register 2
    IMR3      = 0x1B  // Interrupt mapping register 3
  };

  /* Liest ein beliebiges Register des Sensors */
  unsigned char read_register(Register reg);
  unsigned char write_register(Register reg, unsigned char value);

  /* Driver system interface ------------------------------------------------ */
  void plugin();

  // Gate methods
  bool prologue() override;
};

#endif
