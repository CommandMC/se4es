#include "machine/board.h"
#include "machine/bmp085.h"

Board::Revision Board::getRevision() {
    static Revision rev = [] {
        if (BMP085::is_sensor_available()) {
            return Revision::White;
        } else {
            return Revision::Black;
        }
    }();
    return rev;
}
