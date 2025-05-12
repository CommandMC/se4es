#ifndef __STANDBY_MODE_H__
#define __STANDBY_MODE_H__

#include "guard/gate.h"

class StandbyMode : public Gate {
private:
  // Solange schlafen, wie dieses flag nicht gesetzt ist
  volatile bool running;

  static StandbyMode inst;

  StandbyMode(const StandbyMode &copy);
  StandbyMode();
public:
  static StandbyMode& instance() { return inst; }

  void activate();
  
  // check BSL button and reboot if running=true
  bool prologue();

  enum { LIGHT = (1<<3) };
};

#endif /* __STANDBY_MODE_H__ */

