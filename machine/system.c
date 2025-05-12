#include "config.h"
#include <msp430.h>
#include "machine/system.h"

/* Compile-Zeit-Berechnungen, um eine gueltige CPU-Frequenz sicherzustellen */
#define XT1_FREQUENCY  32768
#define FLL_MULTIPLIER ((CONFIG_CPU_FREQUENCY + XT1_FREQUENCY/2) / XT1_FREQUENCY)

#if FLL_MULTIPLIER >= 1024
#  error Kein Multiplikator fuer die gewuenschte Frequenz moeglich!
#endif

#if CONFIG_CPU_FREQUENCY < 32768
#  error Gewuenschte CPU-Frequenz ist zu niedrig!
#endif

/* Zyklen-basierte Verzoegerungsfunktion */
/* Timer an dieser Stelle noch nicht verwendbar, da die Takte noch nicht stabil sind */
static void delay_loop_1(unsigned int loops) {
  /*
           jmp     label         2
           nop                   1
           add     #-1, r15      1
    label: tst     r15           1
           jnz     $-6           2
           ret                   2?
     => 6 + 5*loops Zyklen
  */
  while (loops--) asm volatile("nop");
}

/* Wartet immer etwas zu lange */
#define delay_cycles(cycles) delay_loop_1((cycles) / 5)

static void vcore_increase(unsigned int level) {
  /* Power Managemant-Register entsperren */
  PMMCTL0_H = PMMPW_H;

  /* Neue Schwelle fuer High-Side setzen */
  SVSMHCTL = SVSHE | (SVSHRVL0 * level)
           | SVMHE | (SVSMHRRL0 * level);

  /* Neue Schwelle fuer Low-Side setzen */
  SVSMLCTL = SVSLE | SVMLE | (SVSMLRRL0 * level);

  /* Warten bis Supervisor bereit */
  while ((PMMIFG & SVSMLDLYIFG) == 0) ;

  /* Fehlerflags zuruecksetzen */
  PMMIFG &= ~(SVMLVLRIFG | SVMLIFG);

  /* Neue VCore einstellen */
  PMMCTL0_L = PMMCOREV0 * level;

  /* Warten bis neues VCore-Level erreicht */
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0) ;

  /* low-side auf neue Schwelle setzen */
  SVSMLCTL = SVSLE | (SVSLRVL0 * level);

  /* Power-Management-Register wieder sperren */
  PMMCTL0_H = 0;
}

static void vcore_init(void) {
  /* VCore auf Maximum erhoehen */
  while ((PMMCTL0_L & 3) != 3) {
    vcore_increase((PMMCTL0_L & PMMCOREV_3) + 1);
  }
}

void clocks_init(long cfreq) {
  const int fll_mul = ((cfreq + XT1_FREQUENCY/2) / XT1_FREQUENCY);
  if(fll_mul >= 1024) return;

  /* Pins fuer Quarzverwendung einstellen */
  P5SEL |= 3;

  /* 32kHz Qaurz an, geringste Treiberstaerke, interner Lastkondensator */
  UCSCTL6 = XT2OFF | XCAP_3;

  /* XT1 als FLL-Referenz verwenden, Divisor 1 */
  UCSCTL3 = SELREF__XT1CLK | FLLREFDIV__1;

  /* XT1 fuer ACLK verwenden, DCO als MCLK/SMCLK */
  UCSCTL4 = SELA__XT1CLK | SELS__DCOCLK | SELM__DCOCLK;

  /* Alle Teiler zuruecksetzen */
  UCSCTL5 = DIVPA__1 | DIVA__1 | DIVS__1 | DIVM__1;

  /* Diese Abfragen sollten vom Compiler auf eine Zuweisung reduziert werden */
  if (cfreq      <   530000)
    UCSCTL1 = DCORSEL_0;
  else if (cfreq <  1100000)
    UCSCTL1 = DCORSEL_1;
  else if (cfreq <  2340000)
    UCSCTL1 = DCORSEL_2;
  else if (cfreq <  4600000)
    UCSCTL1 = DCORSEL_3;
  else if (cfreq <  9100000)
    UCSCTL1 = DCORSEL_4;
  else if (cfreq < 17200000)
    UCSCTL1 = DCORSEL_5;
  else
    UCSCTL1 = DCORSEL_6;

  UCSCTL1 = DCORSEL_5;                // DCO range 6..23MHz (worst case)
  UCSCTL2 = FLLD__1 | fll_mul; // Schleifenmultiplikator 1
  _BIC_SR(SCG0);                      // FLL einschalten

  /* Darauf warten, dass die FLL stabil ist - aufgeteilt um Ueberlauf zu vermeiden */
  // FIXME: Sollte vom Takt abhaengen
  delay_cycles(250000);
  delay_cycles(125000);

  /* Warten bis Quarz und DCO stabil sind */
  do {
    /* Fehlerflags zuruecksetzen */
    UCSCTL7 = 0;
    /* "oscillator fault" interrupt flag zuruecksetzen */
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);

  /* XT2 wird vom Radio bei Bedarf automatisch eingeschaltet */

  /* Watchdog einschalten, Ausloeseverzoegerung ca. 50 Sekunden */
  /* Das naechst kleinere Intervall waere ca. 3 Sekunden (WDTIS__32K) */
  WDTCTL = WDTPW | WDTSSEL__VLO | WDTCNTCL | WDTIS__512K;
}

#define CALL_RFBSL()   ((void (*)())0x1000)()

void system_call_bsl(void) {
  __dint();
  asm volatile ("nop"); // CPU-Erratum-Workaround

  /* Display loeschen, in unterer Zeile "bSL" ausgeben */
  LCDBMEMCTL |= LCDCLRM | LCDCLRBM;
  LCDM9  = 0x58;
  LCDM10 = 0x3d;
  LCDM11 = 0x7c;

  // Watchdog auf schnellste Reaktion einstellen
  WDTCTL = WDTPW | WDTSSEL__VLO | WDTCNTCL | WDTIS__64;

  // Warten bis der WDT zuschlaegt, damit der BSL-Trigger aufgerufen wird
  while (1) ;
}


#ifdef TI_COMPILER
__attribute__((section(".crt_0042"), used, naked))
#else
__attribute__ ((section(".init3"), used, naked))
#endif /* TI_COMPILER */
static void check_wdt_reset(void) {
  /* Reset-Grund pruefen, bei Watchdog-Zuschlag BSL aufrufen */
  if (SYSRSTIV == SYSRSTIV_WDTTO) {
    SYSRSTIV = 0;

    /* In unterer Zeile "bSL" ausgeben */
	LCDBMEMCTL |= LCDCLRM | LCDCLRBM;
    LCDM9  = 0x58;
    LCDM10 = 0x3d;
    LCDM11 = 0x7c;

    /* Timer A0 zuruecksetzen, da der BSL sonst nicht laeuft */
    TA0CTL = 0;
    TA0CCTL0 = 0;
    TA0R = 0;
    TA0EX0 = 0;

    __dint();
    asm volatile ("nop"); // CPU-Erratum-Workaround
    CALL_RFBSL();
  }
}

#ifdef TI_COMPILER
__attribute__((section(".crt_0042"), used, naked))
static void disable_watchdog(void) {
  /* Der TI Compiler deaktiviert den Watchdog nicht im Startup-Code. */
  /* Damit der Watchdog nicht vor der main zuschlaegt, wird dieser hier verlangsamt. */
  /* Watchdog einschalten, Ausloeseverzoegerung ca. 50 Sekunden */
  /* Das naechst kleinere Intervall waere ca. 3 Sekunden (WDTIS__32K) */
  WDTCTL = WDTPW | WDTSSEL__VLO | WDTCNTCL | WDTIS__512K;
}
#endif /* TI_COMPILER */

void system_init(void) {
  vcore_init();
  clocks_init(CONFIG_CPU_FREQUENCY);
}

void system_restore(void) {
  vcore_increase(0);
  UCSCTL1 = DCORSEL_0;
  UCSCTL2 = 0;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  UCSCTL6 |= XT1OFF | XT2OFF;
}
