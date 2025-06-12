#include "user/pastthread.h"
#include "machine/system.h"
#include "machine/lcd.h"
#include "syscall/guarded_organizer.h"


void PASTThread::action() {
  Guarded_Organizer& organizer = Guarded_Organizer::instance();
  LCD& lcd = LCD::instance();
  long total_idle_time = 0;

  // Mit voller Geschwindigkeit initialisieren
  long speed_scaled = MAX_SPEED_SCALED;
  long excess_cycles = 0;

  while (1) {
    sleep(UPDATE_INTERVAL_MS);

    // Idle-Zeit holen
    unsigned long idle_cycles = organizer.getLastIdle();
    total_idle_time += idle_cycles;

    // Sicherstellen, dass run_cycles nicht negativ wird, falls idle_cycles > 50 ms
    unsigned long run_cycles = (idle_cycles >= UPDATE_INTERVAL_MS) ? 0 : UPDATE_INTERVAL_MS - idle_cycles;
    run_cycles += excess_cycles;

    // PAST-Algorithmus
    long run_percent_scaled = (run_cycles * SCALE_FACTOR) / UPDATE_INTERVAL_MS;
    long next_excess = (run_cycles * SCALE_FACTOR - speed_scaled * (run_cycles + idle_cycles));

    if (next_excess < 0) {
      next_excess = 0;
    }
    
    long newspeed_scaled = speed_scaled;

    // IF excess_cycles > idle_cycles THEN newspeed = 1.0;
    if (excess_cycles > (long)idle_cycles) {
      newspeed_scaled = MAX_SPEED_SCALED;
    }
    // ELSEIF run_percent > 0.7 THEN newspeed = speed + 0.2;
    else if (run_percent_scaled > HIGH_LOAD_THRESHOLD) {
      newspeed_scaled = speed_scaled + SPEED_INCREASE_STEP;
    }
    // ELSEIF run_percent < 0.5 THEN newspeed = speed – (0.6 – run_percent);
    else if (run_percent_scaled < LOW_LOAD_THRESHOLD) {
      newspeed_scaled = speed_scaled - (SPEED_DECREASE_BASE - run_percent_scaled);
    }
    
    // Neue Geschwindigkeit auf den gültigen Bereich begrenzen.
    if (newspeed_scaled > MAX_SPEED_SCALED) {
      newspeed_scaled = MAX_SPEED_SCALED;
    }
    if (newspeed_scaled < MIN_SPEED_SCALED) {
      newspeed_scaled = MIN_SPEED_SCALED;
    }

    speed_scaled = newspeed_scaled;
    excess_cycles = next_excess;

    // Neue Taktfrequenz berechnen und einstellen
    long new_frequency_hz = (MAX_FREQ_HZ * speed_scaled) / SCALE_FACTOR;
    if (DEBUG_PRINTS) {
      lcd.show_number(speed_scaled, LCD::Line::UPPER);
      lcd.show_number(total_idle_time, LCD::Line::LOWER);
    }
    clocks_init(new_frequency_hz);
  }
}
