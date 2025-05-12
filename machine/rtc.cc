#include "config.h"
#include <msp430.h>

#include "machine/rtc.h"

void RTC::set(time_t time) {
  /* RTC in dezimalen Kalendermodus schalten */
  RTCCTL01 = RTCMODE;

  RTCYEAR = time.year + 2000U;
  RTCMON  = time.month;
  RTCDAY  = time.day;
  RTCHOUR = time.hours;
  RTCMIN  = time.minutes;
  RTCSEC  = time.seconds;
}

void RTC::read(time_t &time) {
  do {
    /* Warten bis die RTC zum Auslesen bereit ist */
    while (!(RTCCTL01 & RTCRDY)) ;

    time.year    = RTCYEAR - 2000U;
    time.month   = RTCMON;
    time.day     = RTCDAY;
    time.hours   = RTCHOUR;
    time.minutes = RTCMIN;
    time.seconds = RTCSEC;

    /* Wiederholen, bis die Uhr auch am Ende bereit war */
  } while (!(RTCCTL01 & RTCRDY));
}
