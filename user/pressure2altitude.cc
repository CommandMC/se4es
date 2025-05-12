#include "user/pressure2altitude.h"

#include <cstdint>
#include <cmath>

#include "machine/lcd.h"
#include "syscall/guarded_tp_sensor.h"

constexpr double press_to_alt(const double pressure) {
    return 44330 * (1 - pow(pressure / 101325, 1 / 5.255));
}

template <unsigned int Min, unsigned int Max>
struct PressureToAltitudeLookupTable {
    int data[Max - Min + 1] = {};
    constexpr PressureToAltitudeLookupTable() {
        for (
            unsigned int i = 0;
            i < Max - Min + 1;
            i++
        ) {
                const unsigned int& pressure = i + Min;
                data[i] = press_to_alt(pressure);
        }
    }
};

constexpr int LOWEST_PRESSURE = 700;
constexpr int HIGHEST_PRESSURE = 1100;
constexpr auto LOOKUP_TABLE = PressureToAltitudeLookupTable<LOWEST_PRESSURE, HIGHEST_PRESSURE>();

void Pressure2Altitude::action() {
    LCD& lcd = LCD::instance();
    Guarded_TPSensor& tp_sensor = Guarded_TPSensor::instance();
    while (true) {
        const long& pressure = tp_sensor.get_pressure();
        lcd.show_number(pressure, LCD::Line::UPPER);

        // 1.3.2
        // lcd.show_number(press_to_alt(pressure), LCD::Line::LOWER);

        // 1.3.4
        if (pressure > HIGHEST_PRESSURE) {
            lcd.show_string("LOW", LCD::Line::LOWER);
        } else if (pressure < LOWEST_PRESSURE) {
            lcd.show_string("HIGH", LCD::Line::LOWER);
        } else {
            const long& index = pressure - LOWEST_PRESSURE;
            lcd.show_number(LOOKUP_TABLE.data[index], LCD::Line::LOWER);
        }
        sleep(250);
    }
}

