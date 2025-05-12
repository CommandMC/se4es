#ifndef CLICKER_H
#define CLICKER_H

#include <cstdint>
#include "syscall/thread.h"

class ClickerThread : public UserThread<512> {
public:
    [[noreturn]] void action() override;

    static ClickerThread& instance() {
        static ClickerThread clicker;
        return clicker;
    }

private:
    uint16_t counter = 0;

    void on_counter_update() const;
    void on_up();
    void on_down();
};

#endif //CLICKER_H
