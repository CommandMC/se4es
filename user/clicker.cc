#include "user/clicker.h"

#include "user/buttonservice.h"
#include "machine/lcd.h"
#include "syscall/guarded_beeper.h"

[[noreturn]] void ClickerThread::action() {
    LCD& lcd = LCD::instance();
    lcd.clear();
    lcd.show_string("CLIC");
    on_counter_update();

    ButtonService& bs = ButtonService::instance();
    while (true) {
        sleep(250);
        const ButtonService::ButtonState state = bs.get_buttons();
        if (state & ButtonService::UP) {
            on_up();
            on_counter_update();
        } else if (state & ButtonService::DOWN) {
            on_down();
            on_counter_update();
        }
    }
}

void ClickerThread::on_up() {
    counter++;
}

void ClickerThread::on_down() {
    counter = 0;
}

void ClickerThread::on_counter_update() const {
    LCD::instance().show_number(counter);
    if (counter && counter % 10 == 0) {
        // Rising frequency, resetting every 100 clicks
        const uint16_t& freq = (counter % 100 / 10 + 1) * 250;
        Guarded_Beeper::instance().beep(freq, 200);
    }
}
