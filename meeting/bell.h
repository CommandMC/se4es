/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 B E L L                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Eine "Glocke" ist eine abstrakte Basisklasse, die das Ausloesen einer     */
/* Aktivitaet nach einer bestimmten Wartezeit erlaubt.                       */
/* Dazu besitzt sie einen Zaehler, der vom "Gloeckner" (Bellringer) ver-     */
/* waltet wird.                                                              */
/*****************************************************************************/

#ifndef __Bell_include__
#define __Bell_include__

#include "object/chain.h"

class Bell : public Chain {
private:
  Bell(const Bell &copy); // Verhindere Kopieren

  unsigned int m_value;

public:
  Bell() : m_value(0) {}

  void wait(unsigned int value) { m_value = value;        }
  unsigned int wait()           { return m_value;         }
  void tick()                   { if (m_value) m_value--; }
  bool run_down()               { return (m_value <= 0);  }

  virtual void ring() = 0;
};

#endif
