#ifndef EXAMPLETHREAD_H
#define EXAMPLETHREAD_H

#include "syscall/thread.h"

class ExampleThread : public UserThread<64> {
public:
  /* Die Methode action() ist der Einstiegspunkt des Threads */
  void action();

  /* Methode zum Zugriff auf die Singleton Instanz */
  static ExampleThread& instance() {
    static ExampleThread example; // Singleton Instanz
    return example;
  }
};

#endif

