/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                   C P U                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implementierung einer Abstraktion fuer den Prozessor.                     */
/* Derzeit wird nur angeboten, Interrupts zuzulassen, zu verbieten oder den  */
/* Prozessor anzuhalten.                                                     */
/*****************************************************************************/

#ifndef __CPU_include__
#define __CPU_include__

#include <msp430.h>
#include <in430.h>
#include "machine/system.h"

class CPU
 {
private:
    CPU(const CPU &copy); // Verhindere Kopieren
    CPU();
public:
    // Erlauben von (Hardware-)Interrupts
    static inline void enable_int ()
     {
       __eint();
     }

    // Interrupts werden ignoriert/verboten
    static inline void disable_int ()
     {
       __dint();
       asm volatile ("nop");
     }

    // Prozessor bis zum naechsten Interrupt anhalten
    static inline void idle ()
      {
        /* Watchdog triggern */
        watchdog_reset();

        __eint();
        asm volatile ("nop");
        LPM2;
        asm volatile ("nop"); // CPU-Erratum-Workaround
      }
      
    // Prozessor bis zum naechsten Interrupt anhalten und groﬂteil der Hardware abschalten
    static inline void sleep ()
      {
        __eint();
        asm volatile ("nop");
        LPM3;
        asm volatile ("nop"); // CPU-Erratum-Workaround
      }

    // Prozessor anhalten
    static inline void halt ()
      {
        __eint();
        asm volatile ("nop");
        LPM4;
        asm volatile ("nop"); // CPU-Erratum-Workaround
      }
 };

#endif
