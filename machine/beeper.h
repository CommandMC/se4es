#ifndef BEEPER_H
#define BEEPER_H

class Beeper {
private:
  Beeper(const Beeper &copy);

protected:
  Beeper();

public:
  /* Parameter: Frequenz in Hertz */
  void beep(unsigned int frequency);

  /* Ton ausschalten */
  void stop();
};

#endif
