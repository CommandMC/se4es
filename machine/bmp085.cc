#include "config.h"
#include <msp430.h>
#include "machine/bmp085.h"
#include "machine/timer.h"
#include "machine/ps.h"
#include <limits>
#include <cstdint>

/*
The bmp085 has a built-in ADC.
The sensor switches automatically to powerdown after each measurement.
*/

#define BMP_CHIP_ID 0x55
#define BMP_REG_CHIP_ID 0xD0
#define BMP_REG_VERSION 0xD1

/* I2C address */
//LSB is r/w-bit, see ps.h
#define BMP_I2C_ADDRESS 0xEE

// Measurement mode values
#define BMP_MEAS_TEMPERATURE 0x2E
#define BMP_MEAS_PRESSURE 0x34

/* Sensor registers */
//control register for measurements
#define BMP_REG_MEAS_CTRL 0xF4
//ADC output register
//returns MSB of output, LSB is located at BMP_REG_ADC_OUT + 1
#define BMP_REG_ADC_OUT 0xF6

//calibration data msb register adresses
//lsb register adress is always next to it (MSB_ADDR + 1)
#define BMP_REG_CALI_AC1 0xAA
#define BMP_REG_CALI_AC2 0xAC
#define BMP_REG_CALI_AC3 0xAE
#define BMP_REG_CALI_AC4 0xB0
#define BMP_REG_CALI_AC5 0xB2
#define BMP_REG_CALI_AC6 0xB4
#define BMP_REG_CALI_B1 0xB6
#define BMP_REG_CALI_B2 0xB8
#define BMP_REG_CALI_MB 0xBA
#define BMP_REG_CALI_MC 0xBC
#define BMP_REG_CALI_MD 0xBE

bool BMP085::init_sensor() {
  this->sensorInitialised = false;

  if (!is_sensor_available()) {
    return false;
  }
  this->read_calibration_data();
  this->sensorInitialised = true;

  return true;
}

bool BMP085::is_sensor_available() {
  static bool available = [] {
    //init connection to pressure sensor
    ps_init();

    // Read ChipID to check if communication is working
    unsigned short status = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CHIP_ID, PS_I2C_8BIT_ACCESS);
    return status == BMP_CHIP_ID;
  }();

  return available;
}

//populates the appropiate structure with the calibration data from the sensor
void BMP085::read_calibration_data() {
  /*parameters AC1-AC6*/
  this->Calibration.ac1 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC1, PS_I2C_16BIT_ACCESS);
  this->Calibration.ac2 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC2, PS_I2C_16BIT_ACCESS);
  this->Calibration.ac3 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC3, PS_I2C_16BIT_ACCESS);
  this->Calibration.ac4 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC4, PS_I2C_16BIT_ACCESS);
  this->Calibration.ac5 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC5, PS_I2C_16BIT_ACCESS);
  this->Calibration.ac6 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_AC6,PS_I2C_16BIT_ACCESS);
  
  /*parameters B1,B2*/
  this->Calibration.b1 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_B1, PS_I2C_16BIT_ACCESS);
  this->Calibration.b2 = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_B2, PS_I2C_16BIT_ACCESS);
  
  /*parameters MB,MC,MD*/
  this->Calibration.mb = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_MB, PS_I2C_16BIT_ACCESS);
  this->Calibration.mc = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_MC, PS_I2C_16BIT_ACCESS);
  this->Calibration.md = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_CALI_MD, PS_I2C_16BIT_ACCESS);
}

//sets the resolution and also speed of the pressure measurement
void BMP085::set_mode(Mode mode) {
  this->currentMode = mode;
}

//@return bool check if data is available in ADC
bool BMP085::data_available() {
  return PS_INT_IN & PS_INT_PIN;
}

//@return long temperature in 0.1°C or -32768 in case of an error
int BMP085::read_temp() {
  if (!this->sensorInitialised) {
    return std::numeric_limits<int>::min();
  }

  // variable names and type from datasheet
  long uncompensated_temperature, x1, x2, temperature;

  // start sampling temperature
  if (!ps_write_register(BMP_I2C_ADDRESS, BMP_REG_MEAS_CTRL, BMP_MEAS_TEMPERATURE)) {
    return std::numeric_limits<int>::min();
  }

  //max conversion time for temperature measurements is 4.5ms
  while (!data_available()) {
    //wait for sensor data
    Timer::delay_us(1000);
  }

  uncompensated_temperature = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_ADC_OUT, PS_I2C_16BIT_ACCESS);

  x1 = ((uncompensated_temperature - this->Calibration.ac6) * (long)this->Calibration.ac5) / (1UL << 15);
  x2 = ((long)this->Calibration.mc * (1 << 11)) / (x1 + this->Calibration.md);
  this->b5 = x1 + x2;
  //temperature in 0.1°C
  temperature = (this->b5 + 8) / (1 << 4);

  return temperature;
}

//dependent on temperature measurement
//it is suggested to periodically trigger a temperature measurement every second to get accurate results
//@return int pressure in 1Pa or -2147483648 in case of an error
long BMP085::read_pressure() {
  if (!this->sensorInitialised) {
    return std::numeric_limits<long>::min(); //max negative value of 4byte long
  }

  //calculate current temp if not already done
  if (this->b5 == 0) {
    this->read_temp();
  }

  // start sampling pressure
  if (!ps_write_register(BMP_I2C_ADDRESS, BMP_REG_MEAS_CTRL, static_cast<unsigned char>(this->currentMode))) {
    return std::numeric_limits<long>::min();
  }

  //max conversion time depends on selected mode, absolute max. 25.5ms
  while (!data_available()) {
    //wait for sensor data
    Timer::delay_us(1000);
  }

  uint16_t oss; //oversampling setting

  switch (this->currentMode) {
    case Mode::STANDARD:
      oss = 1;
      break;
    case Mode::HIGH_RESOLUTION:
      oss = 2;
      break;
    case Mode::ULTRA_HIGH_RESOLUTION:
      oss = 3;
      break;
    case Mode::ULTRA_LOW_POWER:
    default:
      oss = 0;
      break;
  }

  // Get MSB from ADC_OUT_MSB_REG
  uint32_t uncompensated_pressure = ps_read_register(BMP_I2C_ADDRESS, BMP_REG_ADC_OUT, PS_I2C_24BIT_ACCESS) >> (8-oss);

  int32_t pressure, x1, x2, x3, b3, b6;
  uint32_t b4, b7;

  // Add Compensation and convert decimal value to Pa

  b6 = this->b5 - 4000;
  //*****calculate B3************
  x1 = (Calibration.b2 * ((b6 * b6) >> 12)) >> 11;

  x2 = (Calibration.ac2 * b6) >> 11;

  x3 = x1 + x2;

  b3 = (((((int32_t) Calibration.ac1) * 4 + x3) << oss) + 2) / 4;

  //*****calculate B4************
  x1 = (Calibration.ac3 * b6) >> 13;
  x2 = (Calibration.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (Calibration.ac4 * (uint32_t)(x3 + 32768)) >> 15;
    
  b7 = (((uint32_t)uncompensated_pressure - b3) * (50000ul >> oss));
  if (b7 < 0x80000000) {
    pressure = (b7 * 2) / b4;
  } else { 
    pressure = (b7 / b4) * 2;
  }
  
  x1 = (pressure >> 8) * (pressure >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * pressure) >> 16;
  return pressure + ((x1 + x2 + 3791) >> 4);	// pressure in Pa
}
