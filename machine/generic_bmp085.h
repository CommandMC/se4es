#ifndef __GENERIC_BMP085__H__
#define __GENERIC_BMP085__H__

#include "tp_sensor.h"
#include "bmp085.h"


/*
 * Adapter class that implements the generic TPSensor interface for the BMP085
 */
struct GenericBMP085 : public TPSensor, private BMP085 {
    bool init() override;
    bool set_mode(TPSensor::Mode mode) override;
    int get_temperature() override;
    long get_pressure() override;
};

#endif  //!__GENERIC_BMP085__H__
