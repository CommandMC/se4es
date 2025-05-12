#include "config.h"
#include <msp430.h>
#include "machine/scp1000.h"
#include "machine/timer.h"
#include "machine/ps.h"

/* Das im Header ausgelassene Makro fuer DRDY */
#define SCP1000_DRDY_DIR P2DIR

/* I2C-Adresse des Sensors */
#define SCP_I2C_ADDRESS 0x22

/* Register des Sensors */
#define SCP_REVID     0x00
#define SCP_DATAWR    0x01
#define SCP_ADDRPTR   0x02
#define SCP_OPERATION 0x03
#define SCP_OPSTATUS  0x04
#define SCP_RSTR      0x06
#define SCP_STATUS    0x07
#define SCP_DATARD8   0x7f
#define SCP_DATARD16  0x80
#define SCP_TEMPOUT   0x81

/* Indirekte Register des Sensors */
#define SCPI_CFG      0x00
#define SCPI_TWIADD   0x05

/* Werte fuer das SCPI_CFG-Register */
#define SCPI_CFG_HIGHRES 0x05
#define SCPI_CFG_LOWRES  0x0d

/* Operationscodes des Sensors */
#define SCPOP_NONE        0x00
#define SCPOP_RD_INDIRECT 0x01
#define SCPOP_WR_INDIRECT 0x02
#define SCPOP_RD_EEPROM   0x05
#define SCPOP_WR_EEPROM   0x06
#define SCPOP_INIT        0x07
#define SCPOP_SELFTEST    0x0f
/* Messmodi sind in Mode::mode_t definiert */

bool SCP1000::init_sensor() {
  this->sensorInitialised = false;

  /* Der Sensor braucht 60ms nach dem Einschalten */
  Timer::delay_us(60000);

  ps_init();
  // I2C_REN |=   SDA | SCL; //<- das fehlt im ps-i2c

  /* Startup-Status pruefen */
  int i,res;
  for (i=0; i<10; i++) {
    res = ps_read_register(SCP_I2C_ADDRESS, SCP_STATUS, PS_I2C_8BIT_ACCESS);

#ifdef CONFIG_PESSIMIST
    if (res < 0)
      /* Sensor nicht vorhanden */
      return;
#endif

    if ((res & 1) == 0)
      /* Initialisierung fertig */
      break;

    Timer::delay_us(10000);
  }

#ifdef CONFIG_PESSIMIST
  if (i == 10)
    /* Initialisation fehlgeschlagen */
    return;
#endif

#if 0
  /* Auskommentiert, da der Test nach Reinitialisierung fehlschlaegt */
  /* auf EEPROM-Checksummen-Fehler pruefen */
  res = ps_read_register(SCP_I2C_ADDRESS, SCP_DATARD8, PS_I2C_8BIT_ACCESS);
# ifdef CONFIG_PESSIMIST
  if (res < 0)
    /* Sensor hat kein ACK gesendet */
    return;
# endif

  if ((res & 1) == 0) {
    /* EEPROM Checksummen-Fehler */
    return;
  }
#endif

  /* Aufloesung auf 17 Bit setzen */
  ps_write_register(SCP_I2C_ADDRESS, SCP_DATAWR,    SCPI_CFG_LOWRES);
  ps_write_register(SCP_I2C_ADDRESS, SCP_ADDRPTR,   SCPI_CFG);
  ps_write_register(SCP_I2C_ADDRESS, SCP_OPERATION, SCPOP_WR_INDIRECT);

  /* Low-Power-Modus einschalten */
  ps_write_register(SCP_I2C_ADDRESS, SCP_OPERATION, static_cast<unsigned char>(SensorMode::LOW_POWER));

  this->sensorInitialised = true;

  return this->sensorInitialised;
}

bool SCP1000::is_sensor_available() {
  if (!this->sensorInitialised) {
    return this->init_sensor();
  }

  return this->sensorInitialised;
}

bool SCP1000::data_available() {
  return PS_INT_IN & PS_INT_PIN;
}

void SCP1000::trigger_measurement() {
  set_sensor_mode(SensorMode::LOW_POWER);
}

void SCP1000::set_sensor_mode(SensorMode mode) {
  ps_write_register(SCP_I2C_ADDRESS, SCP_OPERATION, static_cast<unsigned char>(mode));
}

void SCP1000::set_trigger_mode(TriggerMode mode) {
  switch (mode) {
    case TriggerMode::LOW_RESOLUTION:
      /* Aufloesung auf 15 Bit setzen */
      ps_write_register(SCP_I2C_ADDRESS, SCP_DATAWR,    SCPI_CFG_LOWRES);
      ps_write_register(SCP_I2C_ADDRESS, SCP_ADDRPTR,   SCPI_CFG);
      ps_write_register(SCP_I2C_ADDRESS, SCP_OPERATION, SCPOP_WR_INDIRECT);
      break;
    case TriggerMode::HIGH_RESOLUTION:
      /* Aufloesung auf 17 Bit setzen */
      ps_write_register(SCP_I2C_ADDRESS, SCP_DATAWR,    SCPI_CFG_HIGHRES);
      ps_write_register(SCP_I2C_ADDRESS, SCP_ADDRPTR,   SCPI_CFG);
      ps_write_register(SCP_I2C_ADDRESS, SCP_OPERATION, SCPOP_WR_INDIRECT);
      break;
    
    default:
      break;
  }
}

int SCP1000::read_temp() {
  unsigned long bytes = ps_read_register(SCP_I2C_ADDRESS, SCP_TEMPOUT, PS_I2C_16BIT_ACCESS);

  if (!bytes)
    return -1;

  return (bytes & 0x3FFF);
}

long SCP1000::read_pressure() {
  long bytes = ps_read_register(SCP_I2C_ADDRESS, SCP_DATARD16, PS_I2C_16BIT_ACCESS);

  if (!bytes)
    return -1;

  bytes |= (ps_read_register(SCP_I2C_ADDRESS, SCP_DATARD8, PS_I2C_8BIT_ACCESS) & 0x07) << 16;

  return bytes;
}
