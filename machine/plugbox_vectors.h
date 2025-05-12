/* Vektorliste der Plugbox, ausgelagert da es der einzige */
/* maschinenspezifische Plugbox-Teil ist.                 */

enum {
  Crypto = 0,       //  0
  RealTimeClock,    //  1
  LCD,              //  2
  Port2,            //  3
  Port1,            //  4
  Timer1_Misc,      //  5
  Timer1_Compare0,  //  6
  DMA,              //  7
  Radio,            //  8
  Timer0_Misc,      //  9
  Timer0_Compare0,  // 10
  ADConverter,      // 11
  UniSerial_B0,     // 12
  UniSerial_A0,     // 13
  Watchdog,         // 14
  Comparator,       // 15
  UserNMI,          // 16
  SystemNMI         // 17
  // , Reset
};

/* Anzahl der Vektoren fuer die die Plugbox Eintraege braucht */
#define PLUGBOX_VECTOR_COUNT 18
