#include "user/pressure2altitude.h"

#include <cstdint>
#include <cmath>

#include "machine/lcd.h"
#include "syscall/guarded_tp_sensor.h"

constexpr int LOWEST_PRESSURE = 700;
constexpr int HIGHEST_PRESSURE = 1100;
// Generated by meta/build_pressure_lookup_table.py
constexpr int PRESSURE_LOOKUP[] = { 3012, 3001, 2990, 2978, 2967, 2956, 2945, 2934, 2923, 2912, 2900, 2889, 2878, 2867, 2856, 2845, 2834, 2823, 2812, 2801, 2790, 2779, 2768, 2757, 2746, 2735, 2724, 2713, 2703, 2692, 2681, 2670, 2659, 2648, 2638, 2627, 2616, 2605, 2594, 2584, 2573, 2562, 2551, 2541, 2530, 2519, 2509, 2498, 2487, 2477, 2466, 2455, 2445, 2434, 2424, 2413, 2403, 2392, 2381, 2371, 2360, 2350, 2339, 2329, 2318, 2308, 2298, 2287, 2277, 2266, 2256, 2246, 2235, 2225, 2214, 2204, 2194, 2183, 2173, 2163, 2152, 2142, 2132, 2122, 2111, 2101, 2091, 2081, 2070, 2060, 2050, 2040, 2030, 2020, 2009, 1999, 1989, 1979, 1969, 1959, 1949, 1939, 1929, 1919, 1909, 1898, 1888, 1878, 1868, 1858, 1848, 1838, 1829, 1819, 1809, 1799, 1789, 1779, 1769, 1759, 1749, 1739, 1729, 1720, 1710, 1700, 1690, 1680, 1670, 1661, 1651, 1641, 1631, 1622, 1612, 1602, 1592, 1583, 1573, 1563, 1553, 1544, 1534, 1524, 1515, 1505, 1495, 1486, 1476, 1467, 1457, 1447, 1438, 1428, 1419, 1409, 1400, 1390, 1381, 1371, 1361, 1352, 1342, 1333, 1324, 1314, 1305, 1295, 1286, 1276, 1267, 1257, 1248, 1239, 1229, 1220, 1210, 1201, 1192, 1182, 1173, 1164, 1154, 1145, 1136, 1127, 1117, 1108, 1099, 1089, 1080, 1071, 1062, 1052, 1043, 1034, 1025, 1016, 1006, 997, 988, 979, 970, 961, 952, 942, 933, 924, 915, 906, 897, 888, 879, 870, 861, 852, 843, 834, 825, 815, 806, 797, 789, 780, 771, 762, 753, 744, 735, 726, 717, 708, 699, 690, 681, 672, 663, 655, 646, 637, 628, 619, 610, 602, 593, 584, 575, 566, 557, 549, 540, 531, 522, 514, 505, 496, 487, 479, 470, 461, 453, 444, 435, 427, 418, 409, 401, 392, 383, 375, 366, 357, 349, 340, 332, 323, 314, 306, 297, 289, 280, 272, 263, 254, 246, 237, 229, 220, 212, 203, 195, 186, 178, 169, 161, 153, 144, 136, 127, 119, 110, 102, 94, 85, 77, 68, 60, 52, 43, 35, 27, 18, 10, 2, -6, -14, -22, -31, -39, -47, -56, -64, -72, -80, -89, -97, -105, -113, -122, -130, -138, -146, -154, -163, -171, -179, -187, -195, -204, -212, -220, -228, -236, -244, -252, -261, -269, -277, -285, -293, -301, -309, -317, -325, -333, -341, -349, -358, -366, -374, -382, -390, -398, -406, -414, -422, -430, -438, -446, -454, -462, -470, -478, -485, -493, -501, -509, -517, -525, -533, -541, -549, -557, -565, -573, -580, -588, -596, -604, -612, -620, -628, -635, -643, -651, -659, -667, -675, -682, -690, };

void Pressure2Altitude::action() {
    LCD& lcd = LCD::instance();
    Guarded_TPSensor& tp_sensor = Guarded_TPSensor::instance();
    while (true) {
        const int pressure = tp_sensor.get_pressure() / 100;
        lcd.show_number(pressure, LCD::Line::UPPER);

        // 1.3.2
        // lcd.show_number(press_to_alt(pressure), LCD::Line::LOWER);

        // 1.3.4
        if (pressure > HIGHEST_PRESSURE) {
            lcd.show_string("LO", LCD::Line::LOWER);
        } else if (pressure < LOWEST_PRESSURE) {
            lcd.show_string("HI", LCD::Line::LOWER);
        } else {
            const long& index = pressure - LOWEST_PRESSURE;
            lcd.show_number(PRESSURE_LOOKUP[index], LCD::Line::LOWER);
        }
        sleep(250);
    }
}

