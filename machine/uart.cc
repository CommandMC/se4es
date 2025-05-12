#include <msp430.h>
#include "device/plugbox.h"
#include "machine/uart.h"

#ifdef CONFIG_UART_POLLED

/* Polling version */
void UART::putc(char c) {
  while (!(UCA0IFG & UCTXIFG)) ;
  UCA0TXBUF = c;

  if (c == '\n')
    putc('\r');
}

void UART::flush() {
}

bool UART::prologue() {
  return false;
}

#else

bool UART::prologue() {
  /* Note: This code assumes that the receive interrupt is disabled */
  if (read_idx == write_idx) {
    /* Buffer is empty, disable interrupt */
    UCA0IE &= ~UCTXIE;
    return false;
  }

  UCA0TXBUF = txbuf[read_idx];
  read_idx = (read_idx + 1) & (sizeof(txbuf)-1);

  if (read_idx == write_idx)
    /* Last byte sent, disable interrupts */
    UCA0IE &= ~UCTXIE;

  return false;
}

void UART::putc(char c) {
  unsigned int t=(write_idx+1) & (sizeof(txbuf)-1);

#ifdef CONFIG_UART_CAN_DEADLOCK
  while (t == read_idx);   // wait for free space
#endif

  UCA0IE &= ~UCTXIE;       // turn off RS232 irq

  if ((read_idx == write_idx) &&
      (UCA0IFG & UCTXIFG)) {
    /* Local buffer empty, transmit buffer empty: Send immediately */
    UCA0TXBUF = c;
  } else {
    /* Can't send immediately, use buffer and enable interrupts */
    txbuf[write_idx] = c;
    write_idx = t;
    UCA0IE |= UCTXIE;
  }

  if (c == '\n')
    putc('\r');
}

void UART::flush() {
  while (read_idx != write_idx) ;
}

#endif

void UART::plugin() {
  /* USCI in Reset-Zustand schalten */
  UCA0CTL1 |= UCSWRST;

  UCA0CTL0 = 0; /* UART modus, 8n1 */
  UCA0CTL1 = UCSWRST | UCSSEL__ACLK;  /* Taktquelle ACLK (32768Hz) */
  
  /* Statische Konfiguration - mies, aber einfacher */
#if CONFIG_UART_BAUDRATE == 9600
  /* 9600 bps */
  UCA0BR0  = 3;
  UCA0MCTL = UCBRS_3;

#elif CONFIG_UART_BAUDRATE == 4800
  /* 4800 bps */
  UCA0BR0  = 6;
  UCA0MCTL = UCBRS_7;

#elif CONFIG_UART_BAUDRATE == 2400
  /* 2400 bps */
  UCA0BR0  = 13;
  UCA0MCTL = UCBRS_6;

#elif CONFIG_UART_BAUDRATE == 1200
  /* 1200 bps */
  UCA0BR0  = 27;
  UCA0MCTL = UCBRS_2;

#else
#  error "Unbekannte UART-Baudrate eingestellt!"
#endif

  /* IrDA und Autobauding abschalten */
  UCA0IRCTL = 0;
  UCA0ABCTL = 0;
  
#ifdef CONFIG_UART_DEBUG
  /* Pinzuweisung aktualisieren */
  PMAPPWD = PMAPPW;
  PMAPCTL = PMAPRECFG; /* Spaetere Umstellungen erlauben */
  P2MAP7  = PM_UCA0TXD;

  P2SEL |= BV(7);
  P2DIR |= BV(7);
#endif

  /* UART einschalten */
  UCA0CTL1 &= ~UCSWRST;

  Plugbox::instance().assign(Plugbox::UniSerial_A0, *this);
}

/* Required for stdio */
extern "C" int putchar(int c) {
  UART::instance().putc(c);
  return 1;
}

void UART::puthex(unsigned char num) {
  int tmp;

  tmp = (num & 0xf0) >> 4;
  if (tmp < 10)
    putc('0'+tmp);
  else
    putc('a'+tmp-10);

  tmp = num & 0x0f;
  if (tmp < 10)
    putc('0'+tmp);
  else
    putc('a'+tmp-10);
}

void UART::puts(char *text) {
  while (*text) {
    putc(*text++);
  }
}
