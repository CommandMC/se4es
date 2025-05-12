/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         G U A R D E D _ R A D I O                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Schnittstelle von Anwendungsthreads zum Radio-Objekt                      */
/*****************************************************************************/

#ifndef GUARDED_RADIO
#define GUARDED_RADIO

#include "machine/radio.h"
#include "guard/secure.h"

class Guarded_Radio : public Radio {
private:
  Guarded_Radio(const Guarded_Radio &copy); // Verhindere Kopieren
  Guarded_Radio() {}

public:
  /* Methode zum Zugriff auf die Singleton Instanz */
  static Guarded_Radio& instance() {
    Secure sec;
    static Guarded_Radio radio; // Singleton Instanz
    return radio;
  }

  /* Liefert die ID des aktuellen Knoten zurueck */
  unsigned long get_id() {
    Secure sec;
    return Radio::get_id();
  }

  /* Schaltet den Empfang ein (Stromfresser!).
     Wenn der Funkchip komplett abgeschaltet wurde (sleep) wird er implizit aufgeweckt.
     Ein Aufruf von awake() ist nicht erforderlich. */
  void enable_rx() {
    Secure sec;
    Radio::enable_rx();
  }

  /* Schaltet den Empfang aus. Der komplette Funkchip wird hierbei NICHT schlafen gelegt.
     Für maximales Stromsparen ist ein Aufruf von sleep() erforderlich. */
  void disable_rx() {
    Secure sec;
    Radio::disable_rx();
  }

  /* Sendet ein Paket an die Broadcast-Adresse, liefert true wenn erfolgreich */
  bool send_broadcast(void *data, unsigned char length) {
    Secure sec;
    return Radio::send_broadcast(data,length);
  }

  /* Empfaengt ein Paket, liefert false wenn keines vorliegt */
  bool receive_broadcast(void *data, unsigned char *lengthptr) {
    Secure sec;
    return Radio::receive_broadcast(data, lengthptr);
  }

  /* Sendet ein Paket ueber einen Link, liefert true wenn erfolgreich */
  bool send(unsigned char link, void *data, unsigned char length) {
    Secure sec;
    return Radio::send(link, data, length);
  }

  /* Empfaengt ein Paket von einem Link, liefert false wenn keines vorliegt */
  bool receive(unsigned char link, void *data, unsigned char *lengthptr) {
    Secure sec;
    return Radio::receive(link, data, lengthptr);
  }

  /* Versucht aktiv einen Link aufzubauen - liefert true wenn erfolgreich.
     Falls erfolgreich, steht danach in 'link' die Adresse des Partners,
     zu dem eine Verbindung besteht. Diese dient dann fuer send/receive */
  bool link(unsigned char &link) {
    Secure sec;
    return Radio::link(link);
  }

  /* Wartet auf einen Linkaufbau - liefert true wenn erfolgreich.
     Falls erfolgreich, steht danach in 'link' die Adresse des Partners,
     zu dem eine Verbindung besteht. Diese dient dann fuer send/receive */
  bool link_listen(unsigned char &link) {
    Secure sec;
    return Radio::link_listen(link);
  }

  /* Versetzt Radio in den Schlafmodus */
  void sleep() {
    Secure sec;
    Radio::sleep();
  }

  /* Weckt Radio aus dem Schlafmodus auf */
  void awake() {
    Secure sec;
    Radio::awake();
  }

};

#endif
