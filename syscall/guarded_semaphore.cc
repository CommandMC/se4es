/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   G U A R D E D _ S E M A P H O R E                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Systemaufrufschnittstelle zum Semaphor.                                   */
/*****************************************************************************/

#include "guard/secure.h"
#include "syscall/guarded_semaphore.h"

Guarded_Semaphore::~Guarded_Semaphore () {
  Secure section;

  Semaphore::clear(); // Wecke alle wartenden Threads
}

void Guarded_Semaphore::p() {
  Secure sec;

  Semaphore::p();
}

void Guarded_Semaphore::v() {
  Secure sec;

  Semaphore::v();
}

