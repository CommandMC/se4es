#ifndef PASTTHREAD_H
#define PASTTHREAD_H

#include "syscall/thread.h"

class PASTThread : public UserThread<512> { // 512 Byte Stack
private:
  const unsigned long UPDATE_INTERVAL_MS = 50;

  // Zum Skalieren von prozentualen Werten
  const long SCALE_FACTOR = 1000;

  const long MAX_FREQ_HZ = 27000000;
  const long MIN_FREQ_HZ = 5000000;

  const long MAX_SPEED_SCALED = SCALE_FACTOR;
  const long MIN_SPEED_SCALED = (MIN_FREQ_HZ * SCALE_FACTOR) / MAX_FREQ_HZ;

  const long HIGH_LOAD_THRESHOLD = 700;
  const long LOW_LOAD_THRESHOLD  = 500;
  const long SPEED_DECREASE_BASE = 600;
  const long SPEED_INCREASE_STEP = 200;

  // Debug Prints ausgeben
  const bool DEBUG_PRINTS = true;

public:
  /* Die Methode action() ist der Einstiegspunkt des Threads */
  void action();

  /* Methode zum Zugriff auf die Singleton Instanz */
  static PASTThread& instance() {
    static PASTThread example; // Singleton Instanz
    return example;
  }
};

#endif

