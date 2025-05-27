#ifndef FP_TEST_THREAD_H
#define FP_TEST_THREAD_H

#include "syscall/thread.h"

class FPTestThread : public UserThread<256> {
public:
    void action() override;

    static FPTestThread& instance() {
        static FPTestThread clicker;
        return clicker;
    }

};

#endif
