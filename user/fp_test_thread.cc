#include "fp_test_thread.h"

#include "machine/timer.h"
#include "machine/lcd.h"

#define USE_FIXED_POINT 1

#if USE_FIXED_POINT == 1
#include "fixedpoint.h"
typedef FixedPoint ess_float;
#else
typedef float ess_float;
#endif

ess_float bench(ess_float f1, ess_float f2) {
    ess_float ret = 0.f;
    for (ess_float i = 10.f; i > 0.f; i -= 0.2f) {
        ret += (i * f2);
    }
    return ret / f1;
}

void FPTestThread::action() {
    LCD& lcd = LCD::instance();

    Timer::start();
    long result = bench(0.8f, 5.2f);
    Timer::stop();

    lcd.show_number(result, LCD::Line::UPPER);
    lcd.show_number(Timer::getcycles());
}
