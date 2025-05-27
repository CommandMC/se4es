/*
 * main.cc - Initialisierung und Threadstart
 *
 * Wenn alle Threads mit Aspekten eingebunden werden, sind
 * in dieser Datei keine Aenderungen notwendig. Wenn nicht,
 * sollten Threadanmeldungen in ready_threads() erfolgen.
 */

#include "device/watch.h"
#include "guard/guard.h"
#include "machine/cpu.h"
#include "machine/system.h"
#include "machine/standby.h"
#include "syscall/guarded_organizer.h"
#include "syscall/thread.h"


// Hier sollten die Includes der eigenen Thread-Headerfiles ergaenzt werden
#include "user/examplethread.h"
#include "user/buttonservice.h"
// #include "user/clicker.h"
// #include "user/pressure2altitude.h"
#include "user/fp_test_thread.h"

/* Threads beim Scheduler anmelden */
static void ready_threads() {
  Guarded_Organizer::instance().ready(ExampleThread::instance());
  Guarded_Organizer::instance().ready(ButtonService::instance());
  // Guarded_Organizer::instance().ready(ClickerThread::instance());
  // Guarded_Organizer::instance().ready(Pressure2Altitude::instance());
  Guarded_Organizer::instance().ready(FPTestThread::instance());
}

int main() {
  // Zuerst in den Stand-By-Modus LPM4 wechseln.
  // Erst wenn der Licht-Knopf gedrueckt wird, geht es hier weiter.
  StandbyMode::instance().activate();

  // VCore und Takte konfigurieren
  system_init();

  // Auf Epilogebene wechseln um ungestoert initialisieren zu koennen
  CPU::disable_int();
  Guard::instance().enter();

  // Timerinterrupts einschalten
  Watch::instance().windup();
  CPU::enable_int();

  // Threads aktivieren
  ready_threads();

  // Scheduling starten
  Guarded_Organizer::instance().Organizer::schedule();

  // Endlosschleife - wird nie erreicht, spart aber ein wenig Speicher
  while (1) ;
}
