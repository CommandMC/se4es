#ifndef RADIO_H
#define RADIO_H

#include "guard/gate.h"
#include "syscall/guarded_semaphore.h"

class Radio : public Gate {
private:
  Radio(const Radio &copy); // Verhindere Kopieren
  bool sleeping;
  bool rx_enabled;

protected:
  Radio() : packet_received(0) { plugin(); }

public:
  /* Interrupthandler registrieren */
  void plugin();

  /* Interner Interrupt-Kram */
  bool prologue();
  void epilogue();

  /*** Benutzerfunktionen ***/

  /* Semaphore, die fuer jedes empfangene Paket ein signal() bekommt */
  Guarded_Semaphore packet_received;

  /* Liefert die ID des aktuellen Knoten zurueck */
  unsigned long get_id();

  /* Schaltet den Empfang ein (Stromfresser!) */
  void enable_rx();

  /* Schaltet den Empfang aus */
  void disable_rx();

  /* Sendet ein Paket ueber einen Link, liefert true wenn erfolgreich */
  bool send(unsigned char link, void *data, unsigned char length);

  /* Empfaengt ein Paket von einem Link, liefert false wenn keines vorliegt */
  bool receive(unsigned char link, void *data, unsigned char *lengthptr);

  /* Versucht aktiv einen Link aufzubauen - liefert true wenn erfolgreich.
     Falls erfolgreich, steht danach in 'link' die Adresse des Partners,
     zu dem eine Verbindung besteht. Diese dient dann fuer send/receive */
  bool link(unsigned char &link);

  /* Wartet auf einen Linkaufbau - liefert true wenn erfolgreich.
     Falls erfolgreich, steht danach in 'link' die Adresse des Partners,
     zu dem eine Verbindung besteht. Diese dient dann fuer send/receive */
  bool link_listen(unsigned char &link);

  /* Sendet ein Paket an die Broadcast-Adresse, liefert true wenn erfolgreich */
  bool send_broadcast(void *data, unsigned char length);

  /* Empfaengt ein Paket, liefert false wenn keines vorliegt */
  bool receive_broadcast(void *data, unsigned char *lengthptr);

  /* Schaltet Radio in Stromsparmodus */
  void sleep();

  /* Weckt Radio nach Verlassen des MCU-Sleep-Modus wieder auf */
  void awake();

};

#endif
