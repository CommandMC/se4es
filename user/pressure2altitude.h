#ifndef PRESSURE2ALTITUDE_H
#define PRESSURE2ALTITUDE_H

#include "syscall/thread.h"

class Pressure2Altitude : public UserThread<128> {
public:
    [[noreturn]] void action() override;

    static Pressure2Altitude& instance() {
        static Pressure2Altitude p2a;
        return p2a;
    }
};

#endif
