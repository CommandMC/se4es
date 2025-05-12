#ifndef UART_H
#define UART_H

#include "guard/gate.h"

// UART an Pin schalten?
//#define CONFIG_UART_DEBUG=y

// Log2 der UART-Buffer-Groesse
#define CONFIG_UART_BUF_SHIFT 6

// UART-Baudrate
#define CONFIG_UART_BAUDRATE 4800

// Deadlocks beim Schreiben erlauben
// (d.h. warten auf freien Platz im Puffer)
#define CONFIG_UART_CAN_DEADLOCK

// Ausgabe mit Polling statt ISR?
//#define CONFIG_UART_POLLED=y


class UART : public Gate {
private:
  UART(const UART &copy); // Verhindere Kopieren
  UART() {}

#ifndef CONFIG_UART_POLLED
  char txbuf[1 << CONFIG_UART_BUF_SHIFT];
  volatile unsigned int read_idx;
  volatile unsigned int write_idx;
#endif

public:
  static UART& instance() {
    static UART uart;
    return uart;
  }

  /* Interrupthandler registrieren */
  void plugin();

  /* Interner Interrupt-Kram */
  bool prologue();
  void epilogue() {};

  /* Benutzerfunktionen */
  virtual void putc(char c);
  void puthex(unsigned char num);
  void flush();
  void puts(char *text);
};

#endif
