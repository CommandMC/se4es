/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                               B U Z Z E R                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Mit Hilfe des "Weckers" koennen Prozesse eine bestimmte Zeit lang         */
/* schlafen und sich dann wecken lassen.                                     */
/*****************************************************************************/

#ifndef __Buzzer_include__
#define __Buzzer_include__

#include "meeting/bell.h"
#include "meeting/waitingroom.h"

class Buzzer : public Waitingroom, public Bell {
private:
  Buzzer(const Buzzer &copy); // Verhindere Kopieren

  unsigned int buzzticks;

protected:
  ~Buzzer() {} // Verhindere direktes zerstoeren
  void clear(); // Wecke alle wartenden Threads

public:
  Buzzer() : buzzticks(0) {}
  virtual void ring();
  void set(unsigned int ms);
  void sleep();
  void remove(Customer *target);
};

#endif
