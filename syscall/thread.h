/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   T H R E A D / U S E R T H R E A D                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zur Erstellung von Threads                      */
/*****************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include "thread/customer.h"
#include "syscall/guarded_buzzer.h"

class Thread : public Customer {
private:
  Thread(const Thread &copy);

public:
  Thread(void *tos) : Customer(tos) {}
};


/* Fehlervermeidender Wrapper zur Deklaration:
 *
 * Die Template-Klasse UserThread<> bekommt als
 * Template-Parameter die gewünschte Stack-Größe
 * und legt intern einen passenden Stack an.
 */

template<unsigned int STACKSIZE>
class UserThread : public Thread {
private:
  UserThread(const UserThread &copy); // verhindere Kopieren
  Guarded_Buzzer buzzer;

protected:
  // Speicherplatz fuer den Stack reservieren
  void* stack[(STACKSIZE + sizeof(void*) - 1) / sizeof(void*)];

  // Diesen Thread fuer "ms" Millisekunden schlafen legen,
  // um Strom zu sparen.
  // Die CPU wird solange in einen Low-Power-Modus versetzt.
  void sleep(unsigned int ms) {
    buzzer.set(ms);
    buzzer.sleep();
  }

public:
  UserThread() : Thread( stack + sizeof(stack)/sizeof(stack[0]) ) {}
};

#endif
