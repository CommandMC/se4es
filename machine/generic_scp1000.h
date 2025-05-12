#ifndef __GENERIC_SCP1000__H__
#define __GENERIC_SCP1000__H__

#include "tp_sensor.h"
#include "scp1000.h"

/*
 * Adapter class that implements the generic TPSensor interface for the SCP1000
 */
struct GenericSCP1000 : public TPSensor, private SCP1000 {
    bool init() override;
    bool set_mode(Mode mode) override;
    int get_temperature() override;
    long get_pressure() override;
};

#endif  //!__GENERIC_SCP1000__H__
