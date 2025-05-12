#include "machine/accel_sensor.h"

#include "machine/cma3000.h"
#include "machine/bma250.h"
#include "machine/board.h"

AccelSensor &AccelSensor::instance() {
    static CMA3000 cma;
    static BMA250 bma;
    static AccelSensor &s = [&]() -> AccelSensor& {
        if (Board::getRevision() == Board::Revision::Black) {
            cma.plugin();
            return cma;
        }
        bma.plugin();
        return bma;
    }();
    return s;
}
