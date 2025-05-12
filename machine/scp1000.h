/* SCP1000 Druck- und Temperatursensor */
#ifndef SCP1000_H
#define SCP1000_H

class SCP1000 {
  bool sensorInitialised = false;

protected:
  SCP1000() = default;

public:
  SCP1000(const SCP1000 &) = delete;

  enum class SensorMode : unsigned char {
    /* 15 Bit Dauermessung, ca. 9Hz */
    HIGH_SPEED      = 0x09,

    /* 17 Bit Dauermessung, ca. 1.8 Hz */
    HIGH_RESOLUTION = 0x0a,

    /* 15 Bit Dauermessung, ca. 1 Hz */
    ULTRA_LOW_POWER = 0x0b,

    /* 17 Bit Einzelmessung */
    LOW_POWER       = 0x0c
  };

  enum class TriggerMode : unsigned char {
    /* 15 Bit Auflösung */
    LOW_RESOLUTION  = 0U,

    /* 17 Bit Auflösung */
    HIGH_RESOLUTION = 1U,
  };

  /* Initialisiert den Sensor */
  bool init_sensor();

  bool is_sensor_available();

  /* Setzt den Auswertemodus */
  void set_sensor_mode(SensorMode mode);

  void set_trigger_mode(TriggerMode mode);

  /* Loest eine einzelne Messung im Low-Power-Modus aus */
  void trigger_measurement();

  /* Prueft, ob die Messung abgeschlossen ist */
  /* Wird durch Auslesen des Drucks zurueckgesetzt */
  bool data_available();

  /* Liest den zuletzt gemessenen Temperaturwert aus */
  int read_temp();

  /* Liest den zuletzt gemessenen Druckwert aus */
  long read_pressure();
};

#endif
