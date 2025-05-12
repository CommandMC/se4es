/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  G A T E                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Klasse von Objekten, die in der Lage sind, Unterbrechungen zu behandeln.  */
/*****************************************************************************/

#ifndef __Gate_include__
#define __Gate_include__

#include "config.h"
#include "object/chain.h"

class Gate : public Chain {
private:
  Gate(const Gate &copy);

protected:
  bool is_queued;

public:
  Gate() : is_queued(false) {}
  virtual bool prologue()=0;
  virtual void epilogue() {}
  void queued(bool q) { is_queued = q; }
  bool queued() { return is_queued; }
};

                
#endif
