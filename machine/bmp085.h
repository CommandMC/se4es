/* Bosch BMP085 Druck- und Temperatursensor */
#ifndef BMP085_H
#define BMP085_H

class BMP085 {
protected:
  BMP085() = default;

public:

  //only the pressure measurement is affected by the resolution setting!
  enum class Mode : unsigned char {
    //16 bit temperature, 16 bit pressure
    //max wait time 4.5ms
    ULTRA_LOW_POWER = 0x34, // 0x34 + (0<<6)

    //16 bit temperature, 17 bit pressure
    //max wait time 7.5ms
    STANDARD       = 0x74, // 0x34 + (1<<6)
    
    // 16 bit temperature, 18 bit pressure
    // max wait time 13.5ms
    HIGH_RESOLUTION = 0xB4, // 0x34 + (2<<6)

    // 16 bit temperature, 19 bit pressure
    // max wait time 25.5ms
    ULTRA_HIGH_RESOLUTION = 0xF4 // 0x34 + (3<<6)
  };

  //initialise sensor
  bool init_sensor();

  static bool is_sensor_available();

  //set resolution of pressure measurements
  void set_mode(Mode mode);

  //checks if data is available in ADC output registers
  bool data_available();

  //measures and calculates the current temperature
  int read_temp();

  //measures and calculates the current pressure dependent on current mode
  long read_pressure();

private:
  BMP085(const BMP085 &copy) = delete;
  
  //holds calibration data
  struct calibration_t {
    short ac1;
    short ac2;
    short ac3;
    unsigned short ac4;
    unsigned short ac5;
    unsigned short ac6;
    short b1;
    short b2;
    short mb;
    short mc;
    short md; 
  } Calibration;

  //internal variable to transmit temperature to pressure calculation
  long b5 = 0;

  bool sensorInitialised = false;

  Mode currentMode = Mode::ULTRA_LOW_POWER;

  //fetches calibration data from sensor
  void read_calibration_data();
};

#endif
