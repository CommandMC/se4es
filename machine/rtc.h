/* Interne RTC */
#ifndef RTC_H
#define RTC_H

class RTC {
private:
  RTC(const RTC &copy);

protected:
  RTC() {}

public:
  /* Zeit-Datenstruktur, alle Felder in BCD! */
  struct time_t {
    unsigned char seconds; /* Sekunden, 0-59    */ // FIXME: Schaltsekunde?
    unsigned char minutes; /* Minuten, 0-59     */
    unsigned char hours;   /* Stunden, 0-23     */
    unsigned char day;     /* Tag, 1-31         */
    unsigned char month;   /* Monat, 1-12       */
    unsigned char year;    /* Jahr (minus 2000) */ // FIXME: Y2256-Problem!
  };

  void set(time_t time);

  void read(time_t &time);
};

#endif
