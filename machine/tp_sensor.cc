#include "machine/tp_sensor.h"

#include "machine/generic_scp1000.h"
#include "machine/generic_bmp085.h"

TPSensor &TPSensor::instance() {
    static GenericSCP1000 scp;
    static GenericBMP085 bmp;
    static TPSensor &tps = [&]() -> TPSensor& {
        if (bmp.init()) {
            return bmp;
        }
        scp.init();
        return scp;
    }();
    return tps;
}
